import numpy as np
import tensorflow as tf
import sys, os

sys.path.append(os.path.dirname(os.path.realpath(__file__)))
from python.include import board_utilities
from python.include import model_saver
from python.models import cnn_deeper
import configparser
import os
import sys

os.environ['KMP_DUPLICATE_LIB_OK'] = 'True'


class Train:

    def __init__(self):
        self.model = cnn_deeper.get_model()
        self.model.summary()
        config = configparser.ConfigParser()
        config.read('settings.ini')
        self.num_games = config.getint('control', 'num_games')
        self.model_version_number = 0
        self.save_model()
        self.frac_real_moves = 0.4

    def iterate(self):
        self.model_version_number += 1
        dead_boards, moves, move_scores, calculated_boards, calculated_scores = self.read_in_data()

        print("Average score for moves:", np.average(move_scores))
        print("Average score for calculated boards:", np.average(calculated_scores))

        dead_scores = np.zeros(dead_boards.shape[0], "float")

        moves_shaped = board_utilities.arr_to_board(moves)
        calculated_boards_shaped = board_utilities.arr_to_board(calculated_boards)
        dead_boards_shaped = board_utilities.arr_to_board(dead_boards)

        current_frac_real_moves = len(moves_shaped) / (len(moves_shaped) + len(dead_boards) + len(calculated_boards))
        num_repeats = round(self.frac_real_moves / current_frac_real_moves)
        if (num_repeats > 1):
            print("Augmenting move data", num_repeats, "times")
            moves_shaped = np.repeat(moves_shaped, num_repeats, axis=0)
            move_scores = np.repeat(move_scores, num_repeats, axis=0)

        data_shaped = np.concatenate((moves_shaped, calculated_boards_shaped, dead_boards_shaped), axis=0)
        scores = np.concatenate((move_scores, calculated_scores, dead_scores), axis=0)

        if len(moves_shaped > 0):
            history = self.model.fit(
                data_shaped,
                scores,
                epochs=1,
                batch_size=128,
                shuffle=True
            )

        self.save_model()

    def read_in_data(self):
        print("Reading in training data...")
        moves_u64 = np.array([], "uint64")
        move_scores = np.array([], "float")
        dead_boards_u64 = np.array([], "uint64")
        calculated_boards_u64 = np.array([], "uint64")
        calculated_scores = np.array([], "uint64")
        for i in range(self.num_games):
            dirname = "game_logs/game" + str(i)
            if not os.path.isdir(dirname):
                print("Directory not found:", dirname)
                continue

            new_moves_u64 = np.fromfile(dirname + "/moves.dat", "uint64")
            new_move_scores = np.empty((new_moves_u64.shape[0]), "float")
            for i in range(new_moves_u64.shape[0]):
                new_move_scores[i] = new_moves_u64.shape[0] - i - 1

            moves_u64 = np.concatenate((moves_u64, new_moves_u64))
            move_scores = np.concatenate((move_scores, new_move_scores))

            dead_boards_u64 = np.concatenate([dead_boards_u64, np.fromfile(dirname + "/dead_boards.dat", "uint64")])
            calculated_boards_u64 = np.concatenate([calculated_boards_u64,
                                                    np.fromfile(dirname + "/calculated_boards.dat", "uint64")])
            calculated_scores = np.concatenate([calculated_scores,
                                                np.fromfile(dirname + "/calculated_scores.dat", "float")])

        print("Found", len(moves_u64), "moves")
        print("Found", len(dead_boards_u64), "dead boards")
        print("Found", len(calculated_boards_u64), "calculated boards")
        print("Found", len(calculated_scores), "calculated scores")

        dead_boards = board_utilities.int_to_arr(dead_boards_u64)
        moves = board_utilities.int_to_arr(moves_u64)
        calculated_boards = board_utilities.int_to_arr(calculated_boards_u64)
        return dead_boards, moves, move_scores, calculated_boards, calculated_scores

    def save_model(self):
        # model_saver.save_json_h5(model, "model.json", "model.h5")
        # model_saver.save_protobuf(model, ".", "testmy.pb")
        model_saver.save_docker(self.model, "models/docker",  self.model_version_number)

    def test_board(self):
        test_board = 0xfedcba9876543210
        test_board_arr = board_utilities.int_to_arr(np.array([test_board], "uint64"))
        board_utilities.print_board(test_board_arr[0])
        test_board_shaped = board_utilities.arr_to_board(test_board_arr)
        out = self.model.predict(test_board_shaped)
        print("========================================")
        board_utilities.print_board(test_board_arr[0])
        print("Prediction:", out)


def main():
    train = Train()
    train.iterate()


if __name__ == "__main__":
    main()
