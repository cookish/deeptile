//
// Created by Martin Cook on 2018-12-04.
//

#include "RunStats.hh"

#include <iostream>
using std::cout;
using std::endl;

RunStats::RunStats(const std::vector<GameStats> &games) {
    auto numGames = games.size();
    numGens = 0;
    for (const auto &game : games) {
        numGens = std::max(numGens, game.numGens);
    }
    if (numGames > 0) {
        for (const auto &game : games) {
            for (const auto &dStat : game.doubleValues) {
                values[dStat.first] += dStat.second / numGames;
            }
            for (const auto &iStat : game.doubleValues) {
                values[iStat.first] += iStat.second * 1. / numGames;
            }
            values["passedCriticalPoint"] += game.passedCriticalPoint ? 1. / numGames : 0;
            for (const auto &gameVals : game.intValuesPerGen) {
                valuesPerGen[gameVals.first].resize(numGens);
                for (int gen = 0; gen < gameVals.second.size(); ++gen) {
                    valuesPerGen[gameVals.first][gen] += gameVals.second[gen] * 1. / numGames;
                }
            }
        }
    }
}

void RunStats::printRateInfo() {
    cout << "Evaluations/s: " << (values["leafEvals"] / values["timeTaken"])
         << ", moves/s: " << (values["moves"] / values["timeTaken"])
         << endl;
}