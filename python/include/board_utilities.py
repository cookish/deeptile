import numpy as np


def print_board(board):
    if board.shape == (4, 4, 1):
        pos_list = [i for i in range(16)]
        board = board.ravel()
    elif board.shape == (16,):
        pos_list = [15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2, 3]
    count = 0
    for pos in pos_list:
        count += 1
        val = 2**board[pos]
        if val == 1:
            val = 0
        print(str(val).ljust(4), end="")
        if count % 4 == 0:
            print("")


def print_board_hex(board):
    for i in range(15, 0, -1):
        if board[i] > 0:
            print(format(board[i], 'x'), end='')
        else:
            print('.', end='')
        if i % 4 == 0 and i > 0:
            print("|", end='')


def int_to_arr(boards):
    boards_split = np.empty((boards.shape[0], 16), "u1")
    for i in range(boards.size):
        board = boards[i]
        for j in range(16):
            boards_split[i][j] = (board >> np.uint64(4 * j)) & np.uint64(15)
    return boards_split


def arr_to_board(arr):
    arr = arr.reshape(-1, 4, 4)[:, ::-1, ::-1]  # reverse whole board
    arr[:, 1::2, :] = arr[:, 1::2, ::-1]        # reverse every second row
    return arr.reshape(-1, 4, 4, 1)