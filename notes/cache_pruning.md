### With clearing:
```
size_t numGames = 50;
size_t numThreads = 5;
int generations = 2;
```


```
Average total: 5220.04
{
    "boardTotal": 5220.039999999999,
    "cacheHitsPerGen": [
        2555173.2,
        467.5400000000002
    ],
    "cacheMissesPerGen": [
        1931783.9599999997,
        100394.33999999998
    ],
    "cachedEvals": 2628282.0999999996,
    "cachedEvalsPerGen": [
        2590707.6599999997,
        37574.44000000001
    ],
    "leafEvals": 1931783.9599999997,
    "moves": 2369.98,
    "nodesPerGen": [
        4486957.16,
        100861.88
    ],
    "score": 51262.96000000001,
    "timeTaken": 6.351360309500001,
    "totalEvals": 4560066.0600000005,
    "totalEvalsPerGen": [
        4522491.62,
        4560066.0600000005
    ]
}
Evaluations/s: 304153, moves/s: 373.145
Overall leaf evals fraction: 0.423631
Per generation:
  Gen 0 > cache hits: 0.569467, cached evals: 0.57285, evals per node: 1.00792
  Gen 1 > cache hits: 0.00463545, cached evals: 0.00823989, evals per node: 45.211
```

### With board score pruning:
```
Average total: 4650.56
{
    "boardTotal": 4650.56,
    "cacheHitsPerGen": [
        2372408.76,
        570.4399999999999
    ],
    "cacheMissesPerGen": [
        1644176.8399999999,
        89315.08000000002
    ],
    "cachedEvals": 2444250.2799999993,
    "cachedEvalsPerGen": [
        2404179.96,
        40070.320000000014
    ],
    "leafEvals": 1644176.8399999999,
    "moves": 2113.2,
    "nodesPerGen": [
        4016585.6000000006,
        89885.52000000002
    ],
    "score": 44410.31999999999,
    "timeTaken": 12.713261769839997,
    "totalEvals": 4088427.1199999996,
    "totalEvalsPerGen": [
        4048356.8,
        4088427.1199999996
    ]
}
Evaluations/s: 129328, moves/s: 166.22
Overall leaf evals fraction: 0.402154
Per generation:
  Gen 0 > cache hits: 0.590653, cached evals: 0.593866, evals per node: 1.00791
  Gen 1 > cache hits: 0.00634629, cached evals: 0.00980091, evals per node: 45.4848
```
So... much slower. Slightly better cache hit rate. Less score (chance?)

##### Implement cached row totals:

```
Average total: 4457.36
{
    "boardTotal": 4457.360000000001,
    "cacheHitsPerGen": [
        2333495.7800000003,
        542.44
    ],
    "cacheMissesPerGen": [
        1601956.7000000007,
        86745.56000000003
    ],
    "cachedEvals": 2402883.8400000003,
    "cachedEvalsPerGen": [
        2364609.4999999995,
        38274.340000000004
    ],
    "leafEvals": 1601956.7000000007,
    "moves": 2025.3400000000004,
    "nodesPerGen": [
        3935452.4799999995,
        87288.0
    ],
    "score": 42445.280000000006,
    "timeTaken": 9.9149689843,
    "totalEvals": 4004840.5400000005,
    "totalEvalsPerGen": [
        3966566.1999999997,
        4004840.5400000005
    ]
}
Evaluations/s: 161570, moves/s: 204.271
Overall leaf evals fraction: 0.400005
Per generation:
  Gen 0 > cache hits: 0.592942, cached evals: 0.596135, evals per node: 1.00791
  Gen 1 > cache hits: 0.00621437, cached evals: 0.00955702, evals per node: 45.8808```
```

Slightly faster... still more work to do.

Conclusion: clear cache every move for now. But revisit when generations are deeper, because then caching might be more valuable.