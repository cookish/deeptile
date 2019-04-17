//
// Created by Martin Cook on 2019-04-17.
//

#include "Settings.hh"

#include "INIReader.h"
#include <iostream>
using std::cout;
using std::endl;

Settings::Settings(const string &iniFileName) {

    INIReader reader(iniFileName);
    if (reader.ParseError() < 0) {
        cout << "Can't load '" << iniFileName << "'\n";
        exit(1);
    }
    // control
    num_games = reader.GetInteger("control", "num_games", 1);
    num_threads = reader.GetInteger("control", "num_threads", 1);

    // expectimax
    minimum_generations = reader.GetInteger("control", "minimum_generations", 2);
    minimum_evaluations_per_move = reader.GetInteger("control", "minimum_evaluations_per_move", 200);
}
