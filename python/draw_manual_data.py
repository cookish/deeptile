import math
import numpy as np
from matplotlib import pyplot as plt
import matplotlib.ticker as ticker
# matplotlib.rcParams.update({'errorbar.capsize': 3})
y_log = False
y_err = [0, 0, 0, 0]

# ============================== Settings =============================== #
x = [1, 2, 3, 4]

# title = "Evaluations / move"
# y = [3774.854000000003 / 148.75400000000005,
#      137842.8080000001 / 198.18399999999983,
#      5099854.276666666 / 211.04666666666662,
#      299216277.5450001 / 226.69500000000014,
#     ]
# x_title = "Generation"
# y_title = "Evals / move"
# y_log = True

# title = "Fraction of total evals that came from cache"
# y = [1 - 0.996676, 1 - 0.504268, 1 - 0.168454, 1 - 0.0290961]
# x_title = "Generation"
# y_title = "Fraction cache evals"

# title = "Fraction passing critical point (8192..512 -> 16384)"
# y = [0.083, 0.184, 0.273333, 0.38]
# num_y = [1000, 500, 300, 200]
# x_title = "Generation"
# y_title = "Fraction passing"
# y_err = []
# for i in range(len(y)):
#    y_err.append(math.sqrt(y[i] / num_y[i]))

title = "Moves per sec"
y = [812.647, 82.1121, 7.40047,  0.657738]
x_title = "Generation"
y_title = "Moves / sec"
y_log = True
# ======================================================================= #


# plt.figure()
fig, ax = plt.subplots(1,1)

plt.errorbar(x, y, yerr=y_err, fmt='--o', capsize=3, linewidth=1)


tick_spacing = 1
ax.xaxis.set_major_locator(ticker.MultipleLocator(tick_spacing))
plt.title(title)
plt.xlabel(x_title)
plt.ylabel(y_title)
if y_log:
    plt.yscale('log')
plt.show()


