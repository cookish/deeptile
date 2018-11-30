
#include "BoardHandler.hh"
#include "ExpectiMax.hh"
#include "Utility.hh"
#include "HeuristicScorer.hh"

#include <iomanip>
#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

using std::make_unique;
using std::make_shared;

Board initBoard(Utility* utility);
string getMoveName(int move);

int main() {
    auto bh = make_shared<BoardHandler>(make_unique<RowHandler>());
    auto utility = make_shared<Utility>();
    ExpectiMax em(bh, utility, make_unique<HeuristicScorer>(bh));
    em.branchLimit = 500;
    em.scoreForDeath = 0;

    auto board = initBoard(utility.get());
//    Board board = 0x0001012111210013;
    bh->printHex(board);
    int move = 0;
    int evalCount = 0;
    int score = 0;
    while (true) {
        em.getBestMoveRecurse(board, move, 1, evalCount);
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
        cout << "Score: " << score << endl;
    }
    cout << "Final score: " << score << endl;
    bh->printHex(board);
    auto x = 1;
    return 0;
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
            break;
        case 1: return "up";
            break;
        case 2: return "right";
            break;
        case 3: return "down";
            break;
        default:
            return "error";
    }
}