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
    num_games = reader.GetInteger("control", "num_games", num_games);
    num_threads = reader.GetInteger("control", "num_threads", num_games);

    // expectimax
    minimum_generations = reader.GetInteger("expectimax", "minimum_generations", minimum_generations);
    minimum_evaluations_per_move = reader.GetInteger("expectimax", "minimum_evaluations_per_move",
        minimum_evaluations_per_move);
    moved_board_prob_thresh_1 = reader.GetReal("expectimax", "moved_board_prob_thresh_1", moved_board_prob_thresh_1);
    moved_board_prob_thresh_2 = reader.GetReal("expectimax", "moved_board_prob_thresh_2", moved_board_prob_thresh_2);
    moved_board_prob_thresh_3 = reader.GetReal("expectimax", "moved_board_prob_thresh_3", moved_board_prob_thresh_3);
    spawned_board_prob_thresh_1 = reader.GetReal("expectimax", "spawned_board_prob_thresh_1",
        spawned_board_prob_thresh_1);
    spawned_board_prob_thresh_2 = reader.GetReal("expectimax", "spawned_board_prob_thresh_2",
        spawned_board_prob_thresh_2);

    //heuristic
    algorithm_option = reader.GetInteger("heuristic", "algorithm_option", algorithm_option);
}
