### Todo
 * [d] Change from 2048 game score to total on board
 * Store num evaluations that went into each cached score
 * Collect better stats per game
   * total number of moves
   * time taken
   * evaluations per sec
   * moves per sec
   * distribution of evaluations per move. Std dev?
   * Distribution of cache hit rates per generation
   * Different types of evaluations per generation... cached evaluation, speculative evaluations (i.e. to judge move probabilities), leaf evaluations
 * Change from counting generations up to counting down. Pass the total number of generations needed.
 * Start pruning the cache. Either generate from scratch each move, or prune boards < current move. This allows sharing between moves, but might make be slower. Need to test empirically.

### Studies to do
 * Better to calc principal board value first, then check score, or to store in cache for each equivalent board position.

 * Clear cache completely per move, or prune out smaller board positions? If prune, how often?

 * Evaluate after move or after spawn or both (two separate caches)?

 * Work out how probability and generations remaining map to number of evaluations, so that we can judge cache quality.

 * "Critical point" studies. Set up a board near a big combinations, e.g. 8k, 4k, 2k, 1k tiles in place, and see how many times it manages to create a 16k tile. Stop then. Games should be super quick.


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

Should try and keep stats correct. First step would be to prune to only one 4 branch, but weight it as <# open tiles> * 0.1, so that the weight of 2s and 4s is preserved.

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
