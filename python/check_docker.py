import requests
import numpy as np
import json

import include.board_utilities as board_utilities

host = 'localhost'
port = '8501'

test_boards = np.empty(shape=1000, dtype="uint64")
for i in range(1000):
    test_boards[i] = 0xfedcba9876543210 + i

test_board_arr = board_utilities.int_to_arr(test_boards)
board_utilities.print_board_hex(test_board_arr[0])
test_board_shaped = board_utilities.arr_to_board(test_board_arr)

print()
print("===============================")
print("Converting board to list")
x = test_board_shaped.tolist()

# print("x:")
# print(x)

print("Converting list to json string")
data = json.dumps({"instances": x})

# headers = {"content-type": "application/json"}
print("Posting string to Docker")
json_response = requests.post("http://%s:%s/v1/models/serving_default:predict" % (host, port), data=data)

print("Received response from Docker")

response = json.loads(json_response.text)
if 'predictions' in response:
    print("================= Predictions: =======================")
    predictions = json.loads(json_response.text)['predictions']
    print('Found', len(predictions), 'predictions')
    for i in range(500, 510):
        print(predictions[i])
else:
    print(json_response)
