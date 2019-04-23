import numpy as np
import struct

def print_board(board):
    for pos in [15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2, 3]:
        val = 2**board[pos]
        if val == 1:
            val = 0
        print(str(val).ljust(4), end="")
        if pos in [12, 11, 4, 3]:
            print("")


def print_board_hex(board):
    for i in range(15, 0, -1):
        if board[i] > 0:
            print(format(board[i], 'x'), end='')
        else:
            print('.', end='')
        if i % 4 == 0 and i > 0:
            print("|", end='')


def split_boards(boards):
    boards_split = np.empty((boards.shape[0], 16), "u1")
    for i in range(boards.size):
        board = boards[i]
        for j in range(16):
            boards_split[i][j] = (board >> np.uint64(4 * j)) & np.uint64(15)
    return boards_split


print("Reading in data...")
moves_u64 = np.fromfile("thread0/moves.dat", "uint64")
print("Found", len(moves_u64), "moves")
dead_boards_u64 = np.fromfile("thread0/dead_boards.dat", "uint64")
print("Found", len(dead_boards_u64), "dead boards")
calculated_boards_u64 = np.fromfile("thread0/calculated_boards.dat", "uint64")
print("Found", len(calculated_boards_u64), "calculated boards")
calculated_scores = np.fromfile("thread0/calculated_scores.dat", "float")
print("Found", len(calculated_scores), "calculated scores")

dead_boards = split_boards(dead_boards_u64)
moves = split_boards(moves_u64)
calculated_boards = split_boards(calculated_boards_u64)

for i in range(6):
    print_board_hex(moves[i])
    print()
