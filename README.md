# deeptile
AI to play the 2048 game. Deep learning, self-teaching. 

This is a fun project, inspired by Alpha-Go, to create a 2048 AI that plays against itself, training a neural network to evaluate board positions.

I love the concept of emerging intelligence, without providing human wisdom.

## Rough Roadmap

1. Get decent performance without the AI, using a set algorithm to evaluate board positions.
1. Get a reasonable degree of optimisation
1. Experiment with various options to do with tree pruning, result caching
1. Automate the cross-over between game and neural net. Each set of games generates a set of board positions and labels, which trains the AI, which improves for the next set of games
1. Optimise AI, try different architectures etc.

## AI strategy

Suggest getting the AI to predict the number of moves left before death. Perhaps log(moves).  
When evaluating in the expectimax tree, each combination of boards can be a new training board.
