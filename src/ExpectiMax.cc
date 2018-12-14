//
// Created by Martin Cook on 2018/11/30.
//

#include "ExpectiMax.hh"

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
    spawnedBoardsToProcess.clear();
    spawnedBoardChildren.resize(gens+1);
    movedBoardChildren.resize(gens+1);
    genMvdScores.resize(gens+1);
    genSpwndScores.resize(gens+1);

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


            auto moveOptions = getPrunedMoves2(spBoard, cumulProb, currentGen);

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

int ExpectiMax::getBestMove(Board &newBoard) {
    if (genSpwndScores.back().empty()) {
        return -1;
    }
    auto root = genSpwndScores.back().begin();
    auto bestScore = root->second;
    auto board = root->first;
    newBoard = board;
    for (int m = 0; m < 4; ++m) {
        newBoard = bh->getPrincipalBoard(bh->moveLeft(board));
        auto testScore = genMvdScores[genMvdScores.size() - 1][newBoard];
        if (std::abs(genMvdScores[genMvdScores.size() - 1][newBoard] - bestScore) < 0.000001 * bestScore) {
            return m;
        }
        board = bh->rotateLeft(board);
    }
    return -1;
}

void ExpectiMax::printTree() {
    int gens = spawnedBoardChildren.size() - 1;
    int indent = 0;
    cout << std::hex;
    for (int currentGen = gens; currentGen > 0; --currentGen) {
        cout << "\n-------------------------------------------------------------------------------------------\n";
        cout << "GENERATION " << currentGen;
        cout << "\n-------------------------------------------------------------------------------------------\n";

        for (const auto &spBoardVal : spawnedBoardChildren[currentGen]) {
            for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
            cout << "Spd Board: ";
            bh->printHex(spBoardVal.first);
            cout << " has the following moves:";
            for (const auto &child : spBoardVal.second) {
                cout << "  ";
                bh->printHex(child);
            }
            cout << "\n";
        }
        cout << endl;
        for (const auto &mvBoardVal : movedBoardChildren[currentGen]) {
            for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
            cout << "Mvd Board: ";
            bh->printHex(mvBoardVal.first);
            cout << " has the following spawns:";
            int i = 0;
            for (const auto &child : mvBoardVal.second) {
                if (i % 4 == 0) {
                    cout << "\n";
                    for (int i = 0; i < (gens - currentGen)*4; ++i) cout << " ";
                }
                printf("  %.3e * ", child.prob);
                bh->printHex(child.board);
                ++i;

            }
            cout << endl;
        }
    }
}

vector<ExpectiMax::BoardProb> ExpectiMax::getPrunedMoves2(Board board, const double prob, int gens) const {
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

double ExpectiMax::getBestMoveRecurse(const Board board,
                                      int &move,
                                      const int gens,
                                      int &numEvals,
                                      const double prob,
                                      const int indent)
{
    move = -1;
    numEvals = 0;

    auto possibleMoves = bh->getPossibleMoves(board);
    if (possibleMoves.empty()) {
        return scoreForDeath;
    }

    if (gens == 0) {
        numEvals = 1;
        ++stats->leafEvals;
        return scorer->getScoreSpawned(board);
    }
    //    for (int i = 0; i < indent; i++) cout << " ";
    //    cout << "getBestMoveRecurse: evaluating board:" << endl;
    //    bh->printHex(board, indent);
    auto moveOptions = getPrunedMoves(prob, possibleMoves, gens);
    double score;
    double bestScore = scoreForDeath;
    for (const auto &moveOption : moveOptions) {
        int tmpEvals = 0;
        score = getAverageSpawnRecurse(moveOption.board, gens, tmpEvals, prob * moveOption.prob, indent + 2);
        numEvals += tmpEvals;
        if (score > bestScore) {
            move = moveOption.move;
            bestScore = score;
        }
    }
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getBestMoveRecurse: returning score:" << bestScore << endl;
    return bestScore;
}


double ExpectiMax::getAverageSpawnRecurse(const Board board,
                                          const int gens,
                                          int &numEvals,
                                          const double prob,
                                          int indent)
{
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: evaluating board:" << endl;
//    bh->printHex(board, indent);
    Board spawnedBoard;
    auto spawns = getPrunedSpawns(board, prob);

    int cachedEvals = 0;
    int cacheHits = 0;
    int cacheMisses = 0;
    int numNodes = 0;
    double score = 0;
    int tempMove = 0;
    auto newGens = gens - 1;
    for (const auto &spawn : spawns) {
//        spawnedBoard = board | (static_cast<Board>(spawn.tile) << (4 * spawn.pos));
        auto pBoard = bh->getPrincipalBoard(spawnedBoard);
        auto cacheVal = cache->get(pBoard, newGens);
        ++numNodes;
        if (cacheVal.score >= 0) {  // found a cached result
//            score += cacheVal.score * spawn.prob;
            cachedEvals += cacheVal.numEvals;
            ++cacheHits;
        } else {
            int tmpEvals = 0;
//            auto calcScore = getBestMoveRecurse(
//                spawnedBoard, tempMove, newGens, tmpEvals, prob * spawn.prob, indent + 2);
            numEvals += tmpEvals;
//            score += calcScore * spawn.prob;
//            cache->insertScoreSafe(pBoard, newGens, calcScore, tmpEvals);
            ++cacheMisses;
        }
    }
    numEvals += cachedEvals;
    stats->cachedEvalsPerGen[newGens] += cachedEvals;
    stats->totalEvalsPerGen[newGens] += numEvals;
    stats->cachedEvals += cachedEvals;
    stats->cacheHitsPerGen[newGens] += cacheHits;
    stats->cacheMissesPerGen[newGens] += cacheMisses;
    stats->nodesPerGen[newGens] += numNodes;

//    score = score / possibleSpawns.size();
//    for (int i = 0; i < indent; i++) cout << " ";
//    cout << "getAverageSpawnRecurse: returning score:" << score << endl;
    return score;
}

void ExpectiMax::pruneCache(Board board) {
    cache->prune(board, bh.get());
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
        auto randPos = utility->randInt(numSpawns);
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(2) << (4 * randPos)));
        ret[spBoard] += 0.1;

    } else {
        auto randPos = utility->randInt(numSpawns);
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(1) << (4 * randPos)));
        ret[spBoard] += 0.9;
        randPos = utility->randInt(numSpawns);
        spBoard = bh->getPrincipalBoard(board | (static_cast<uint64_t>(1) << (4 * randPos)));
        ret[spBoard] += 0.1;
    }
    return ret;
}

vector<ExpectiMax::BoardMoveProb>
ExpectiMax::getPrunedMoves(const double prob, const std::vector<BoardAndMove> &possibleMoves, const int gens) const
{
    vector<BoardMoveProb> ret;
    if (possibleMoves.empty()) {
        return ret;
    }
    ret.reserve(possibleMoves.size());



    for (const auto &move : possibleMoves) {
        double score;
        if (gens < 2) {
            ++stats->fastMoveProbCalcs;
            score = scorer->getScoreMovedFast(move.board);
        } else {
            ++stats->moveProbCalcsPerGen[gens];
            score = scorer->getScoreMoved(move.board);
        }
        ret.emplace_back(move.board, move.move, score);
    }

    // TODO: prob is not equal to heuristic score...
    //  should get more nuanced relation by looking at score vs. which move is chosen
    int optionLimit;
    double probThresh;
    if (prob >= 1./100.) {
        optionLimit = 4;
        probThresh = 0.;
    } else if (prob >= 1./4000) {
        optionLimit = 3;
        probThresh = 0.25;
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
    for (int i=0; i < static_cast<int>(possibleMoves.size()) - optionLimit; ++i) {
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
