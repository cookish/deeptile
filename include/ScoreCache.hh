//
// Created by Martin Cook on 2018/11/30.
//

#ifndef MLTWO_SCORECACHE_HH
#define MLTWO_SCORECACHE_HH

#include "BoardHandler.hh"

#include <unordered_map>
using std::unordered_map;

struct CacheVal {
    double score;
    int gen;
    int numEvals;
};

class ScoreCache
{
public:
    explicit ScoreCache(std::shared_ptr<BoardHandler> bh) : bh(std::move(bh)) {;}
    CacheVal get(Board pBoard, int gen) const;
    void insertScoreSafe(Board pBoard, int gen, double score, int numEvals);
    void insertScore(Board pBoard, int gen, double score, int numEvals);

private:
    unordered_map<Board, CacheVal> cache;
    std::shared_ptr<BoardHandler> bh;
};


#endif //MLTWO_SCORECACHE_HH
