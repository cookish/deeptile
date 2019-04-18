//
// Created by Martin Cook on 2019-04-17.
//

#ifndef DEEPTILE_SETTINGS_HH
#define DEEPTILE_SETTINGS_HH

#include <string>
using std::string;

class Settings
{
public:
    explicit Settings(const string &iniFileName);

    // control
    int num_threads = 0;
    int num_games = 0;

    // expectimax
    int minimum_generations = 0;
    int minimum_evaluations_per_move = 0;
    double moved_board_prob_thresh_1 = 0.;
    double moved_board_prob_thresh_2 = 0.;
    double moved_board_prob_thresh_3 = 0.;
    double spawned_board_prob_thresh_1 = 0.;
    double spawned_board_prob_thresh_2 = 0.;

    // heuristic
    int algorithm_option = -1;
    double score_for_death = 0.;

};


#endif //DEEPTILE_SETTINGS_HH
