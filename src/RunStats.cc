//
// Created by Martin Cook on 2018-12-04.
//

#include "RunStats.hh"

RunStats::RunStats(const std::vector<GameStats> &games) {
    auto numGames = games.size();

    if (numGames > 0) {
        auto numGens = games[0].cacheHitsPerGen.size();
        cacheHitsPerGen.resize(numGens);
        cacheMissesPerGen.resize(numGens);
        totalEvalsPerGen.resize(numGens);
        cachedEvalsPerGen.resize(numGens);
        for (const auto &game : games) {
            d["score"] += game.score * 1. / numGames;
            d["boardTotal"] += game.boardTotal * 1. / numGames;
            d["timeTaken"] += game.timeTaken * 1. / numGames;
            d["moves"] += game.moves * 1. / numGames;
            d["cachedEvals"] += game.cachedEvals * 1. / numGames;
            d["leafEvals"] += game.leafEvals * 1. / numGames;
            d["totalEvals"] += game.totalEvals * 1. / numGames;
            for (int i = 0; i < cachedEvalsPerGen.size(); ++i) {
                cacheHitsPerGen[i] += game.cacheHitsPerGen[i] * 1. / numGames;
                cacheMissesPerGen[i] += game.cacheMissesPerGen[i] * 1. / numGames;
                totalEvalsPerGen[i] += game.totalEvalsPerGen[i] * 1. / numGames;
                cachedEvalsPerGen[i] += game.cachedEvalsPerGen[i] * 1. / numGames;
            }
        }
        size_t nonZeroGens = 0;
        for (nonZeroGens=0; nonZeroGens<numGens; ++nonZeroGens) {
            if (cacheHitsPerGen[nonZeroGens] == 0
                && cacheMissesPerGen[nonZeroGens] == 0
                && totalEvalsPerGen[nonZeroGens] == 0
                && cachedEvalsPerGen[nonZeroGens] == 0)
            {
                break;
            }
        }
        if (nonZeroGens != numGens) {
            cacheHitsPerGen.resize(nonZeroGens);
            cacheMissesPerGen.resize(nonZeroGens);
            totalEvalsPerGen.resize(nonZeroGens);
            cachedEvalsPerGen.resize(nonZeroGens);
        }
    }
}