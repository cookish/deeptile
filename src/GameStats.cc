//
// Created by Martin Cook on 2018-12-04.
//

#include "GameStats.hh"

void GameStats::addVal(string name, int val) { intValues[name] += val; }
void GameStats::addVal(string name, double val) { doubleValues[name] += val; }
void GameStats::setVal(string name, int val) { intValues[name] = val; }
void GameStats::setVal(string name, double val) { doubleValues[name] = val; }
void GameStats::addValForGen(string name, int gen, int val) {
    if (gen > numGens) {
        for (auto &valPerGen : intValuesPerGen) {
            valPerGen.second.resize(gen+1);
        }
        numGens = gen + 1;
    }
    if (intValuesPerGen[name].size() < gen + 1) {
        intValuesPerGen[name].resize(gen+1);
    }
    intValuesPerGen[name][gen] += val;
}