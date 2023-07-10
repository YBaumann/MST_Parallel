# MST_Parallel
## Important Notes

This is a personal project that solves the Minimum Spanning Tree Problem in parallel. The task itself is rather simple and there exit libraries that do this fast (e.g. GBBS or NetworkX), the main goal here was to not use any dependencies except the *for directive* by OpenMP. This means, that even parallel reductions are implemented natively. While the code is not near perfect optimization, we were able to achieve a speedup when running on the Euler Cluster by ETH Zurich.

## Requirements

We used the following software:

* GCC version 8.1
* OpenMP version 3.0

### Inputs

We use the NetworkX library to generate random graphs, which can be used to test the Minimum Spanning Tree Algorithm.
