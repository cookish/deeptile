
#include "BoardHandler.hh"
#include "ExpectiMax.hh"
#include "Utility.hh"
#include "HeuristicScorer.hh"
#include "MLScorer.hh"
#include "GameStats.hh"
#include "RunStats.hh"
#include "JsonStats.hh"
#include "Output.hh"
#include "Settings.hh"

#include <boost/filesystem.hpp>
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
        const Settings &settings,
        const shared_ptr<Utility> &utility,
        const shared_ptr<BoardHandler> &bh,
        int threadNo,
        int gameNo);
void runThread(vector<Board> &boards,
               std::mutex &boardMutex,
               vector<GameStats> &results,
               std::mutex &resultMutex,
               const Settings &settings,
               const std::shared_ptr<Utility> &utility,
               const std::shared_ptr<BoardHandler> &bh,
               int threadNo);

int main() {
    Settings settings("settings.ini");
    boost::filesystem::remove_all(settings.prevGameLogDir);
    if (boost::filesystem::exists(settings.gameLogDir)) {
        boost::filesystem::rename(settings.gameLogDir, settings.prevGameLogDir);
    }

    auto bh = make_shared<BoardHandler>(make_unique<RowHandler>());
    auto utility = make_shared<Utility>();

    std::mutex boardMutex;
    std::mutex resultMutex;

    std::vector<std::thread> threads;
    std::vector<Board> boards;
    std::vector<GameStats> results;
    threads.reserve(settings.num_games);
    boards.reserve(settings.num_games);
    results.reserve(settings.num_games);
    {
        std::lock_guard<std::mutex> lock(boardMutex);
        for (int i = 0; i < settings.num_games; ++i) {
            boards.emplace_back(initBoard(utility.get()));
        }
    }
    for (int i = 0; i < settings.num_threads; ++i) {
        threads.emplace_back(runThread, std::ref(boards), std::ref(boardMutex),
                             std::ref(results), std::ref(resultMutex),
                             std::ref(settings), std::ref(utility), std::ref(bh), i);
    }
    for (auto &thread : threads) {
        thread.join();
    }
    auto overallTotal = 0.;
    for (const auto &result : results) {
        auto total = bh->getBoardTotal(result.finalBoard);
        overallTotal += total;
        cout << "Total: " << total << endl;
        cout << Output::formatBoard(result.finalBoard);
    }
    cout << "Average total: " << overallTotal / settings.num_games << endl;
    RunStats runStats(results);
    auto js = JsonStats::create(runStats);
    std::cout << js.dump(4) << std::endl;
    runStats.printRateInfo();
    cout << "Passing critical point: " << runStats.values["passedCriticalPoint"] << endl;
    return 0;
}

void runThread(vector<Board> &boards,
               std::mutex &boardMutex,
               vector<GameStats> &results,
               std::mutex &resultMutex,
               const Settings &settings,
               const std::shared_ptr<Utility> &utility,
               const std::shared_ptr<BoardHandler> &bh,
               int threadNo)
{
    string name = string("Thread ") + std::to_string(threadNo);
    while (true) {
        int gameNumber;
        Board board;
        {
            std::lock_guard<std::mutex> lock(boardMutex);
            if (boards.empty()) {
                return;
            }
            gameNumber = settings.num_games - boards.size();
            board = boards.back();
            boards.pop_back();
        }
        auto result = runGame(board, settings, utility, bh, threadNo, gameNumber);
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            results.emplace_back(*std::move(result));
            cout << name << " completed game " << gameNumber
                 << ", total: " << bh->getBoardTotal(result->finalBoard) << endl;
        }
    }
}

unique_ptr<GameStats>
runGame(Board startBoard,
        const Settings &settings,
        const shared_ptr<Utility> &utility,
        const shared_ptr<BoardHandler> &bh,
        int threadNo,
        int gameNo)
{
    string name = string("Thread ") + std::to_string(threadNo);
    string dirname = settings.gameLogDir + "/game" + std::to_string(gameNo);
    boost::filesystem::create_directories(dirname);
    auto gameLogFile = std::ofstream((dirname + "/game_log.txt").c_str(), std::ios::out);
    if (!gameLogFile) {
        cout << "Warning: could not make file:" << dirname + "/game_log.txt" << endl;
    }

    bool passedCritialPoint = false;
    auto board = startBoard;
    auto utility2 = std::make_shared<Utility>();
    auto dataLogger = std::make_shared<DataLogger>();
    ExpectiMax em(bh,
                  utility2,
                  make_unique<HeuristicScorer>(bh, &settings),
//                  make_unique<MLScorer>(bh, &settings, settings.docker_start_port + threadNo),
                  make_unique<GameStats>(),
                  dataLogger,
                  &settings);
    em.scoreForDeath = 0;
    int move = 0;
    double score = 0;
    int i;

    auto start = steady_clock::now();
//    auto start = clock();
    int totalEvals = 0;
    auto verbosity = 1;
    if (verbosity > 0) {
        cout << "Starting game:" << gameNo << endl;
    }
    if (verbosity > 1) {
        cout << "Starting board:" << endl;
        cout << Output::formatBoard(board);
    }
    for (i = 0; true; ++i) {
        dataLogger->logMove(bh->getPrincipalBoard(board));
        int numEvals = 1;

        int gens_now = settings.minimum_generations;
        while (numEvals > 0 && numEvals < 200 && gens_now < 20) {
            numEvals = em.createTree(board, gens_now++);
        }
        em.scoreLeaves();
        score = em.evaluateTree();
        auto moveInfo = em.getMoves(board);
        totalEvals += numEvals;
        if (moveInfo.empty()) break;
        move = moveInfo.front().move;

        if (verbosity > 3) {
            em.evaluateEffort();
            em.printTree(2);
        }

        gameLogFile << Output::formatMoveInfo(board, moveInfo) << "\n";
        if (verbosity > 1)  {
            cout << Output::formatMoveInfo(board, moveInfo);
            cout << endl;
        }
        score += bh->moveAndScore(board, move);
        if (verbosity > 2)  cout << Output::formatBoard(board);
        auto possibleTiles = bh->getPossibleSpawns(board);

        auto placement = utility2->randInt(static_cast<int>(possibleTiles & 0xFull));
        auto place = bh->getSpawnFromList(possibleTiles, placement);
        if (verbosity > 2)  cout << "Putting tile in place " << place << endl;
        board |= (utility2->coinToss(0.9) ? (1ull << (4 * place)) : (2ull << (4 * place)));


//        if (i % 500 == 0) {
//            cout << name << " >> " << " move: " << i << ", score: " << score
//                 << ", numEvals: " << numEvals << endl;
//        }
        // used to see if board crosses critical point
        if (bh->getHighestTile(board) == 14) {
            passedCritialPoint = true;
//            cout << name << " passed critical point" << endl;
//            break;
        }
    }
    dataLogger->writeToFiles(
        dirname + "/dead_boards.dat",
        dirname + "/calculated_boards.dat",
        dirname + "/calculated_scores.dat",
        dirname + "/moves.dat"
    );
    if (verbosity > 0) {
        cout << "Finished game:" << gameNo << endl;
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
    auto board = 0ull;
//    auto board = Board{0xdcba'9000'0000'0000ull};

    auto pos1 = utility->randInt(16);
    while (BoardHandler::getTileValue(board, pos1) > 0) pos1 = utility->randInt(16);

    auto pos2 = utility->randInt(16);
    while (BoardHandler::getTileValue(board, pos2) > 0 || pos1 == pos2) pos2 = utility->randInt(16);

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
