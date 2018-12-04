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
            score += game.score * 1. / numGames;
            boardTotal += game.boardTotal * 1. / numGames;
            timeTaken += game.timeTaken * 1. / numGames;
            moves += game.moves * 1. / numGames;
            cachedEvals += game.cachedEvals * 1. / numGames;
            leafEvals += game.leafEvals * 1. / numGames;
            for (int i = 0; i < cachedEvalsPerGen.size(); ++i) {
                cacheHitsPerGen[i] += game.cacheHitsPerGen[i] * 1. / numGames;
                cacheMissesPerGen[i] += game.cacheMissesPerGen[i] * 1. / numGames;
                totalEvalsPerGen[i] += game.totalEvalsPerGen[i] * 1. / numGames;
                cachedEvalsPerGen[i] += game.cachedEvalsPerGen[i] * 1. / numGames;
            }
        }
    }
}