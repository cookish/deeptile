//
// Created by Martin Cook on 2018/11/30.
//

#include "ExpectiMax.hh"
#include "Output.hh"

#include <algorithm>
#include <iostream>
#include <ExpectiMax.hh>
#include <iomanip>

using std::cout;
using std::endl;

int ExpectiMax::createTree(Board board, int gens) {
    spawnedBoardChildren.clear();
    movedBoardChildren.clear();
    genMvdScores.clear();
    genSpwndScores.clear();
    genMvdEffort.clear();
    genSpwndEffort.clear();
    spawnedBoardsToProcess.clear();
    spawnedBoardChildren.resize(gens+1);
    movedBoardChildren.resize(gens+1);
    genMvdScores.resize(gens+1);
    genSpwndScores.resize(gens+1);
    genMvdEffort.resize(gens+1);
    genSpwndEffort.resize(gens+1);

    auto pBoard = bh->getPrincipalBoard(board);
    Board newBoard;

    spawnedBoardsToProcess[pBoard] = 1.; // add root node

    for (int currentGen = gens; currentGen > 0; --currentGen) {
        movedBoardsToProcess.clear();

        // process spawned boards
        auto &spChildren = spawnedBoardChildren[currentGen];
        for (const auto &spBoardProb : spawnedBoardsToProcess) {
            auto spBoard = spBoardProb.first;
            auto cumulProb = spBoardProb.second;


            auto moveOptions = getPrunedMoves(spBoard, cumulProb, currentGen);
            for (const auto &moveOption : moveOptions) {
                newBoard = bh->getPrincipalBoard(moveOption.board);
                spChildren[spBoard].emplace_back(newBoard);
                movedBoardsToProcess[newBoard] += moveOption.prob * cumulProb;
            }
        }
        spawnedBoardsToProcess.clear();

        // process moved boards
        auto &mvChildren = movedBoardChildren[currentGen];
        for (const auto &mvBoardProb : movedBoardsToProcess) {
            auto mvBoard = mvBoardProb.first;
            auto cumulProb = mvBoardProb.second;


            auto spawns = getPrunedSpawns(mvBoard, cumulProb);
            for (const auto &spawn : spawns) {
                newBoard = spawn.first;
                mvChildren[mvBoard].emplace_back(newBoard, spawn.second);
                if (currentGen > 1) {
                    spawnedBoardsToProcess[newBoard] += spawn.second * cumulProb;
                } else {
                    genSpwndScores[0][newBoard] = 0.;
                }
            }
        }
    }
    // return the number of leaves
    return static_cast<int>(genSpwndScores[0].size());
}

void ExpectiMax::scoreLeaves() {
    for (auto &leaf: genSpwndScores[0]) {
        if (bh->isDead(leaf.first)) {
            leaf.second = scoreForDeath;
        } else {
            leaf.second = scorer->getScoreSpawned(leaf.first);
        }
    }
}

double ExpectiMax::evaluateTree() {
    auto numGens = movedBoardChildren.size() - 1;
    for (int currentGen = 1; currentGen <= numGens; ++currentGen) {
        Board board;

        // go through moved boards, score = average of spawned children
        for (const auto &boardChildren : movedBoardChildren[currentGen]) {
            double score = 0;
            board = boardChildren.first;
            for (const auto &child : boardChildren.second) {
                score += child.prob * genSpwndScores[currentGen - 1][child.board];
            }
            genMvdScores[currentGen][board] = score;
        }

        // go through spawned boards, score = max of moved children
        for (const auto &boardChildren : spawnedBoardChildren[currentGen]) {
            double maxScore = 0;
            double score;
            board = boardChildren.first;
            for (const auto &child : boardChildren.second) {
                score = genMvdScores[currentGen][child];
                maxScore = std::max(score, maxScore);
            }
            genSpwndScores[currentGen][board] = maxScore;
        }
    }
    // return the score of the tree root (the only spawned board at the highest level)
    if (genSpwndScores.back().empty()) {
        return -1.;
    }
    auto root = genSpwndScores.back().begin();
    return root->second;
}

void ExpectiMax::evaluateEffort() {
    auto numGens = movedBoardChildren.size() - 1;
    for (int currentGen = 1; currentGen <= numGens; ++currentGen) {
        Board board;

        // go through moved boards, effort = sum of effort of spawned children
        for (const auto &boardChildren : movedBoardChildren[currentGen]) {
            int effort = 0;
            board = boardChildren.first;
            for (const auto &child : boardChildren.second) {
                effort += (currentGen == 1 ? 1 : genSpwndEffort[currentGen - 1][child.board]);
            }
            genMvdEffort[currentGen][board] = effort;
        }

        // go through spawned boards, effort = sum of effort of moved children
        for (const auto &boardChildren : spawnedBoardChildren[currentGen]) {
            int effort = 0;
            board = boardChildren.first;
            for (const auto &child : boardChildren.second) {
                effort += (currentGen == 1 ? 1 : genMvdEffort[currentGen][child]);
            }
            genSpwndEffort[currentGen][board] = effort;
        }
    }
}


int ExpectiMax::getBestMove(Board board) {
    if (genSpwndScores.back().empty()) {
        return -1;
    }
    auto root = genSpwndScores.back().begin();
    auto bestScore = root->second;
//    cout << "Best score: " << bestScore << ", gen: " << genSpwndScores.size() - 1 << endl;
//    cout << "Options: ";
    int retMove = -1;
    for (int m = 0; m < 4; ++m) {
        auto movedBoard = bh->moveLeft(board);
        if (movedBoard != board) {

            auto movedPBoard = bh->getPrincipalBoard(movedBoard);
            auto boardScore = genMvdScores[genMvdScores.size() - 1][movedPBoard];
//            cout << utility->getMoveName(m) << " (" << boardScore << ") ";
            if (std::abs(genMvdScores[genMvdScores.size() - 1][movedPBoard] - bestScore) < 0.000001 * bestScore) {
                retMove = m;
            }
        }
        board = bh->rotateLeft(board);
    }
//    cout << endl;
    return retMove;
}

void ExpectiMax::printTree(int numGens) {
    int gens = static_cast<int>(spawnedBoardChildren.size()) - 1;
    cout << std::hex;

    int count = 0;
    for (int currentGen = gens; currentGen > 0; --currentGen) {
        ++count;
        if (count > numGens) break;
        cout << "\n-------------------------------------------------------------------------------------------\n";
        cout << "GENERATION " << currentGen;
        cout << "\n-------------------------------------------------------------------------------------------\n";

        for (const auto &spBoardVal : spawnedBoardChildren[currentGen]) {
            for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
            cout << "Spd Board: ";
            cout << Output::formatBoardHex(spBoardVal.first);
            cout << " has the following moves:";
            for (const auto &child : spBoardVal.second) {
                cout << "  ";
                cout << Output::formatBoardHex(child);
                cout << " (" << genMvdScores[currentGen][child] << ")";
                cout << "[" << genMvdEffort[currentGen][child] << "]";
            }
            cout << "\n";
        }
        cout << endl;
        for (const auto &mvBoardVal : movedBoardChildren[currentGen]) {
            for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
            cout << "Mvd Board: ";
            cout << Output::formatBoardHex(mvBoardVal.first);
            cout << " has the following spawns:";
            int i = 0;
            for (const auto &child : mvBoardVal.second) {
                if (i % 4 == 0) {
                    cout << "\n";
                    for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
                }
                printf("  %.3e * ", child.prob);
                cout << Output::formatBoardHex(child.board);
                cout << " (" << genSpwndScores[currentGen - 1][child.board] << ")";
                cout << "[" << genSpwndEffort[currentGen - 1][child.board] << "]";
                ++i;

            }
            cout << endl;
        }
    }
}

vector<ExpectiMax::BoardProb> ExpectiMax::getPrunedMoves(Board board, double prob, int gens) const {
    Board movedBoard;
    vector<ExpectiMax::BoardProb> ret;

    for (int m = 0; m < 4; ++m) {
        movedBoard = bh->moveLeft(board);
        if (movedBoard != board) {
            movedBoard = bh->getPrincipalBoard(movedBoard);
            for (const auto &b: ret) if (b.board == movedBoard) goto skip_board;

            double score;
            if (gens < 2) {
                stats->addValForGen("fastMoveProbCalcs", gens);
                score = scorer->getScoreMovedFast(movedBoard);
            } else {
                stats->addValForGen("slowMoveProbCalcs", gens);
                score = scorer->getScoreMoved(movedBoard);
            }
            ret.emplace_back(movedBoard, score);
        }
        skip_board:;
        if (m != 4) board = bh->rotateLeft(board);  // don't need to rotate last time
    }
    if (ret.empty()) return ret;

    int optionLimit;
    double probThresh;
    if (prob >= 1./100.) {
        optionLimit = 4;
        probThresh = 0.;
    } else if (prob >= 1./4000) {
        optionLimit = 4;
        probThresh = 0.1;
    } else if (prob > 1./150000) {
        optionLimit = 3;
        probThresh = 0.2;
    } else {
        optionLimit = 2;
        probThresh = 0.4;
    }

    // sort
    std::sort(ret.begin(), ret.end(), []( const auto& lhs, const auto& rhs )
    {
        return lhs.prob > rhs.prob;
    });

    // prune out extras
    int sz = static_cast<int>(ret.size());
    for (int i=0; i < sz - optionLimit; ++i) {
        ret.pop_back();
    }

    // filter out probs that are too low
    auto totalScore = 0.;
    for (const auto &b : ret) totalScore += b.prob;
    while (ret.back().prob / totalScore < probThresh) {
        ret.pop_back();
    }

    // normalise
    totalScore = 0;
    for (const auto &b : ret) totalScore += b.prob;
    for (auto &b : ret) b.prob /= totalScore;

    return ret;
}

unordered_map<Board, double> ExpectiMax::getPrunedSpawns(const Board board, const double prob) const {
    auto possibleSpawns = bh->getPossibleSpawns(board);
    if (possibleSpawns == 0) {
        return {};
    }
    unordered_map<Board, double> ret;

    Board spBoard;
    int numSpawns = static_cast<int>(possibleSpawns) & 0xF;
    if (prob >= 1./400.) {
        for (int i = 0; i < numSpawns; ++i) {
            auto pos = bh->getSpawnFromList(possibleSpawns, i);
            spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(1) << (4 * pos)));
            ret[spBoard] += 0.9 / numSpawns;
            spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(2) << (4 * pos)));
            ret[spBoard] += 0.1 / numSpawns;
        }
    } else if (prob >= 1./4000.) {
        for (int i = 0; i < numSpawns; ++i) {
            auto pos = bh->getSpawnFromList(possibleSpawns, i);
            spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(1) << (4 * pos)));
            ret[spBoard] += 0.9 / numSpawns;
        }
        auto randPos = bh->getSpawnFromList(possibleSpawns, utility->randInt(numSpawns));
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(2) << (4 * randPos)));
        ret[spBoard] += 0.1;

    } else {
        auto randPos = bh->getSpawnFromList(possibleSpawns, utility->randInt(numSpawns));
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(1) << (4 * randPos)));
        ret[spBoard] += 0.9;
        randPos = bh->getSpawnFromList(possibleSpawns, utility->randInt(numSpawns));
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(2) << (4 * randPos)));
        ret[spBoard] += 0.1;
    }
    return ret;
}
