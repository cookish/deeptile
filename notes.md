### Thoughts

Cannot compare postSpawn scores to postMove scores

Should not compare heuristics of different depths... The deeper ones will have more score, so the algorithm will try to seek deeper paths, for example limiting number of moves. Not branch on possible moves?

### Pruning the tree
##### Count 4s
Keep track of number of 4s.  
Keep track of generations remaining  
Based on number of 4s, and generations remaining, Monte Carlo prune branches down


##### Prune based on probability
Keep track of branch probability   
Based on branching ratio, Monte Carlo prune branches down. Less accurate score per branch.

##### Move probabilities based on history
Keep track of most common moves, prune down move options (i.e. if we have been moving left and down often, then don’t consider up or right). Keep the n most popular moves that are possible.

##### Move probabilities based on scores
If a potential board has a much lower principal score than current board (i.e. it moved the big tile off the corner), then it is much less likely. Weight potential moves based on principal board values.

Or even apply heuristic at each step, and weight according to heuristic score.

## Cache:
Store num evaluations that went into a score.  
If we are doing probability pruning, derive formula that relates probability and generations remaining to cache quality. Judge cache quality by number of evaluations that went into a move?

## Ideas:
Go from depth first to breadth first


After a move, keep track of total evaluations  
While (total evaluations < threshold), restart with gen+1
