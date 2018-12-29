
#include "BoardHandler.hh"
#include "ExpectiMax.hh"
#include "Utility.hh"
#include "HeuristicScorer.hh"
#include "ScoreCache.hh"
#include "GameStats.hh"
#include "RunStats.hh"
#include "JsonStats.hh"

#include <mutex>
#include <thread>
#include <iomanip>
#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <ctime>
using std::clock;

#include <chrono>
using std::chrono::steady_clock;

using std::make_unique;
using std::make_shared;

Board initBoard(Utility* utility);
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
    size_t numGames = 20;
    size_t numThreads = 5;
    int generations = 4;

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
    cout << "Passing critical point: " << runStats.values["passedCriticalPoint"] << endl;
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
    bool passedCritialPoint = false;
    auto board = startBoard;
    auto utility2 = std::make_shared<Utility>();
    ExpectiMax em(bh, utility2, make_unique<HeuristicScorer>(bh), make_unique<ScoreCache>(bh), make_unique<GameStats>());
    em.scoreForDeath = 0;
    int move = 0;
    double score = 0;
    int i;

    auto start = steady_clock::now();
//    auto start = clock();
    int totalEvals = 0;
    auto verbosity = 0;
    cout << "Starting board:" << endl;
    bh->printBoard(board);
    for (i = 0; true; ++i) {
        int numEvals = 0;

        int gens_now = gens;
        while (numEvals < 2000 && move >= 0 && gens_now < 20) {
//            em.getBestMoveRecurse(board, move, gens_now++, numEvals, 1.);
            numEvals = em.createTree(board, gens_now++);
        }
        em.scoreLeaves();
        score = em.evaluateTree();
//            Board movedBoard;
//        bh->printHex(board);
//        cout << endl;
        move = em.getBestMove(board);
//            em.printTree();
//            em.pruneCache(board);
//            if (gens_now > 5) {cout << "gens:" << gens_now << endl;}
//        }

        totalEvals += numEvals;
        if (move < 0) break;
        if (verbosity > 0)  cout << "Moving " << utility->getMoveName(move) << endl;
        score += bh->moveAndScore(board, move);
        if (verbosity > 1)   bh->printBoard(board);
        auto possibleTiles = bh->getPossibleSpawns(board);

        auto placement = utility2->randInt(static_cast<int>(possibleTiles & 0xFull));
        auto place = bh->getSpawnFromList(possibleTiles, placement);
        if (verbosity > 1)  cout << "Putting tile in place " << place << endl;
        board |= (utility2->coinToss(0.9) ? (1ull << (4 * place)) : (2ull << (4 * place)));
//        if (i % 100 == 0) {
        if (verbosity > 0)  bh->printBoard(board);
//            cout << name << " >> " << " move: " << i << ", score: " << score
//                 << ", numEvals: " << numEvals << endl;
//        }
        // used to see if board crosses critical point
        if (bh->getHighestTile(board) == 14) {
            passedCritialPoint = true;
            break;
        }
    }
    auto timeTaken = std::chrono::duration_cast<std::chrono::duration<double> >
         (steady_clock::now() - start).count();
    //    auto timeTaken = (clock() - start) / (double) CLOCKS_PER_SEC;
//    cout << name << " >> " << " move: " << i << ", score: " << score << endl;
    auto stats = em.getFinalStats();
    stats->finalBoard = board;
    stats->startBoard = startBoard;
    stats->setVal("score", score);
    stats->setVal("boardTotal", bh->getBoardTotal(stats->finalBoard));
    stats->setVal("moves", i);
    stats->setVal("timeTaken", timeTaken);
    stats->setVal("totalEvals", totalEvals);
    stats->passedCriticalPoint = passedCritialPoint;
    return stats;
}


Board initBoard(Utility* utility) {
    auto board = Board{0xDCBA9ull << (4 * 11)};
//    auto board = Board{0};

    auto pos1 = utility->randInt(12);
    int pos2 = pos1;

    while (pos2 == pos1) {
        pos2 = utility->randInt(12);
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
