import numpy as np
import tensorflow as tf

import include.board_utilities as board_utilities
import include.model_saver as model_saver
import models.cnn_deeper as cnn_deeper

import os
import sys

os.environ['KMP_DUPLICATE_LIB_OK'] = 'True'


def main():
    if len(sys.argv) < 2:
        print("Usage: python train.py <model_version_number>")
        exit(1)



    model_version_number = int(sys.argv[1])

    dead_boards, moves, calculated_boards = read_in_data()

    move_scores = np.empty((moves.shape[0]), "float")
    for i in range(moves.shape[0]):
        move_scores[i] = moves.shape[0] - i - 1

    moves_shaped = board_utilities.arr_to_board(moves)

    model = cnn_deeper.get_model()
    model.summary()

    history = model.fit(
        moves_shaped,
        move_scores,
        epochs=10,
        batch_size=128
    )

    save_model(model, model_version_number)


def read_in_data():
    print("Reading in training data...")
    moves_u64 = np.array([], "uint64")
    dead_boards_u64 = np.array([], "uint64")
    calculated_boards_u64 = np.array([], "uint64")
    calculated_scores = np.array([], "uint64")
    for i in range(1):
        dirname = "game_logs/game" + str(i)
        moves_u64 = np.concatenate([moves_u64, np.fromfile(dirname + "/moves.dat", "uint64")])
        dead_boards_u64 = np.concatenate([dead_boards_u64, np.fromfile(dirname + "/dead_boards.dat", "uint64")])
        # calculated_boards_u64 = np.concatenate([calculated_boards_u64,
        #                                         np.fromfile(dirname + "/calculated_boards.dat", "uint64")])
        # calculated_scores = np.concatenate([calculated_scores,
        #                                     np.fromfile(dirname + "/calculated_scores.dat", "float")])

    print("Found", len(moves_u64), "moves")
    print("Found", len(dead_boards_u64), "dead boards")
    print("Found", len(calculated_boards_u64), "calculated boards")
    print("Found", len(calculated_scores), "calculated scores")

    dead_boards = board_utilities.int_to_arr(dead_boards_u64)
    moves = board_utilities.int_to_arr(moves_u64)
    calculated_boards = board_utilities.int_to_arr(calculated_boards_u64)
    return dead_boards, moves, calculated_boards


def save_model(model, version_number):
    model_saver.save_json_h5(model, "model.json", "model.h5")
    model_saver.save_protobuf(model, ".", "testmy.pb")
    model_saver.save_docker(model, "docker/models",  version_number)


def test_board(model):
    test_board = 0xfedcba9876543210
    test_board_arr = board_utilities.int_to_arr(np.array([test_board], "uint64"))
    board_utilities.print_board(test_board_arr[0])
    test_board_shaped = board_utilities.arr_to_board(test_board_arr)
    out = model.predict(test_board_shaped)
    print("========================================")
    board_utilities.print_board(test_board_arr[0])
    print("Prediction:", out)


if __name__ == "__main__":
    main()
