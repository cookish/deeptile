import sys, os
# print(os.path.dirname(os.path.realpath(__file__)))
sys.path.append(os.getcwd())
# sys.path.append(os.path.dirname(os.path.realpath(__file__)) + '/../cmake-build-debug/')
from python.include import board_utilities
from python.models import cnn_deeper
import heuristicMod
import numpy as np
from sklearn.model_selection import train_test_split
import math

num_games = 16
moves_u64 = np.array([], "uint64")
moves_left_scores = np.array([], "uint64")

for i in range(num_games):
    dirname = "game_logs/game" + str(i)
    if not os.path.isdir(dirname):
        print("Directory not found:", dirname)
        continue

    new_moves_u64 = np.fromfile(dirname + "/moves.dat", "uint64")
    new_move_scores = np.empty((new_moves_u64.shape[0]), "float")
    moves_u64 = np.concatenate((moves_u64, new_moves_u64))
    for i in range(new_moves_u64.shape[0]):
        new_move_scores[i] = new_moves_u64.shape[0] - i - 1
    moves_left_scores = np.concatenate((moves_left_scores, new_move_scores))


print("Found", len(moves_u64), "moves")

move_scores = np.empty(moves_u64.shape, "uint64")
scorer = heuristicMod.Scorer("settings.ini")
# for i in range(len(moves_u64)):
    # move_scores[i] = math.log(scorer.getScoreSpawned(moves_u64[i])+1)
    # move_scores[i] = scorer.getScoreSpawned(moves_u64[i])

moves = board_utilities.int_to_arr(moves_u64)
moves_shaped = board_utilities.arr_to_board(moves)

model = cnn_deeper.get_model()
model.summary()

# Split the data
x_train, x_valid, y_train, y_valid = train_test_split(moves_shaped, moves_left_scores, test_size=0.33, shuffle=True)

if len(moves_shaped > 0):
    history = model.fit(
        x_train,
        y_train,
        epochs=20,
        batch_size=128,
        shuffle=True,
        validation_data=(x_valid, y_valid)
    )

moves = board_utilities.int_to_arr(moves_u64)

t = np.array([x_valid[0]])

print("=====================================")
for i in range(100):
    board_utilities.print_board(x_valid[i])
    print("Actual score:", y_valid[i])
    print("Predicted score:", model.predict(np.array([x_valid[i]]))[0])
    print("\n")
