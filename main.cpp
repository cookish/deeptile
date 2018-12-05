
#include "BoardHandler.hh"
#include "ExpectiMax.hh"
#include "Utility.hh"
#include "HeuristicScorer.hh"
#include "ScoreCache.hh"
#include "GameStats.hh"
#include "RunStats.hh"
#include "JsonStats.hh"

#include <thread>
#include <iomanip>
#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <ctime>
using std::clock;

using std::make_unique;
using std::make_shared;

Board initBoard(Utility* utility);
string getMoveName(int move);
unique_ptr<GameStats>
runGame(Board startBoard,
        int gens,
        const shared_ptr<Utility> &utility,
        const shared_ptr<BoardHandler> &bh,
        string name);
void runThread(vector<Board> &boards, std::mutex &boardMutex,
               vector<GameStats> &results, std::mutex &resultMutex,
               int gens,
               const std::shared_ptr<Utility> &utility,
               const std::shared_ptr<BoardHandler> &bh,
               string name);

int main() {
    size_t numGames = 50;
    size_t numThreads = 5;
    int generations = 2;

    auto bh = make_shared<BoardHandler>(make_unique<RowHandler>());
    auto utility = make_shared<Utility>();

    std::mutex boardMutex;
    std::mutex resultMutex;

    std::vector<std::thread> threads;
    std::vector<Board> boards;
    std::vector<GameStats> results;
    threads.reserve(numGames);
    boards.reserve(numGames);
    results.reserve(numGames);
    for (int i = 0; i < numGames; ++i) {
        boards.emplace_back(initBoard(utility.get()));
    }
    for (int i = 0; i < numThreads; ++i) {
        string name = "Thread " + std::to_string(i);
        threads.emplace_back(runThread, std::ref(boards), std::ref(boardMutex),
                             std::ref(results), std::ref(resultMutex),
                             generations, std::ref(utility), std::ref(bh), name);
    }
    for (auto &thread : threads) {
        thread.join();
    }
    auto overallTotal = 0.;
    for (const auto &result : results) {
        auto total = bh->getBoardTotal(result.finalBoard);
        overallTotal += total;
        cout << "Total: " << total << endl;
        bh->printBoard(result.finalBoard);
    }
    cout << "Average total: " << overallTotal / numGames << endl;
    RunStats runStats(results);
    auto js = JsonStats::create(runStats);
    std::cout << js.dump(4) << std::endl;
    runStats.printRateInfo();
    runStats.printCacheInfo();
    return 0;
}

void runThread(vector<Board> &boards, std::mutex &boardMutex,
               vector<GameStats> &results, std::mutex &resultMutex,
               int gens,
               const std::shared_ptr<Utility> &utility,
               const std::shared_ptr<BoardHandler> &bh,
               string name)
{
    while (true) {
        Board board;
        {
            std::lock_guard<std::mutex> lock(boardMutex);
            if (boards.empty()) {
                return;
            }
            board = boards.back();
            boards.pop_back();
        }
//        cout << name << " starting on board " << std::hex << board << std::dec << endl;
        auto result = runGame(board, gens, utility, bh, name);
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            results.emplace_back(*std::move(result));
            cout << name << " completed game " << results.size()
            << ", total: " << bh->getBoardTotal(result->finalBoard) << endl;
        }
    }
}

unique_ptr<GameStats>
runGame(Board startBoard,
        int gens,
        const shared_ptr<Utility> &utility,
        const shared_ptr<BoardHandler> &bh,
        string name) {
    auto board = startBoard;
    ExpectiMax em(bh, utility, make_unique<HeuristicScorer>(bh), make_unique<ScoreCache>(bh), make_unique<GameStats>());
    em.scoreForDeath = 0;
    int move = 0;
    int evalCount = 0;
    double score = 0;
    int i;

    auto start = clock();
    int totalEvals = 0;
    for (i = 0; true; ++i) {
        int numEvals = 0;

        em.getBestMoveRecurse(board, move, gens, numEvals, evalCount);
        em.pruneCache(board);
        totalEvals += numEvals;
        if (move < 0) break;
//        cout << "Moving " << getMoveName(move) << endl;
        score += bh->moveAndScore(board, move);
//        bh->printHex(board);
        auto possibleTiles = bh->getPossibleSpawns(board);

        auto placement = utility->randInt(static_cast<int>(possibleTiles.size()));
        auto place = possibleTiles[placement];
//        cout << "Putting tile in place " << place << endl;
        board |= (utility->coinToss(0.9) ? (1ull << (4 * place)) : (2ull << (4 * place)));
//        bh->printHex(board);
//        if (i % 500 == 0) {
//            cout << name << " >> " << " move: " << i << ", score: " << score
//                 << ", numEvals: " << numEvals << endl;
//        }
    }
    auto timeTaken = (std::clock() - start) / (double)CLOCKS_PER_SEC;
//    cout << name << " >> " << " move: " << i << ", score: " << score << endl;
    auto stats = em.getFinalStats();
    stats->finalBoard = board;
    stats->startBoard = startBoard;
    stats->score = score;
    stats->boardTotal = bh->getBoardTotal(stats->finalBoard);
    stats->moves = i;
    stats->timeTaken = timeTaken;
    stats->totalEvals = totalEvals;
    return stats;
}


Board initBoard(Utility* utility) {
    auto board = Board{0};
    auto pos1 = utility->randInt(16);
    int pos2 = pos1;

    while (pos2 == pos1) {
        pos2 = utility->randInt(16);
    }
    for (const auto& p : {pos1, pos2}) {
        if (utility->coinToss(0.9)) {
            board |= 1ull << (p * 4);
        }
        else {
            board |= 2ull << (p * 4);
        }
    }
    return board;
}

string getMoveName(int move) {
    switch(move) {
        case 0: return "left";
        case 1: return "up";
        case 2: return "right";
        case 3: return "down";
        default:
            return "error";
    }
}
