//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_SCORECACHE_HH
#define MLTWO_SCORECACHE_HH

#include "BoardHandler.hh"

#include <unordered_map>
using std::unordered_map;

struct ScoreGen {
    double score;
    int gen;
};

class ScoreCache
{
public:
    explicit ScoreCache(std::shared_ptr<BoardHandler> bh) : bh(std::move(bh)) {;}
    double getScore(Board pBoard, int gen) const;
    void insertScoreSafe(Board pBoard, int gen, double score);
    void insertScore(Board board, int gen, double score);

private:
    unordered_map<Board, ScoreGen> cache;
    std::shared_ptr<BoardHandler> bh;
};


#endif //MLTWO_SCORECACHE_HH
