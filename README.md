# MST_Parallel

This is a Repo that stores all files related to any work regarding Parallel Algorithms and Graph creation.



Work-Log

---------

29/11/20\
Today we worked on adjusting the parallel implementation of boruvka. We try to optimizie caching with the help of the chaching papers.
We further run the Graphcreation for a 1M Vertex Graph with K = 120

Self note:
As a next step I could set up the boost lib and compare the runtime of my code vs theirs for mst.


---------

26/11/20\
I parallelized the single loop in Boruvkastep without any careful adjustements.
I ran a simple test and took the mean runtime, this to my surprise I got a nice speedup for parallel implementation.

This could also be because of other things than parallelization.

---------

25/11/20

I set up the files for Boruvka parallel and environment for my laptop.

Self note:
Apparently we need a settings.json file for the intellisense to work in VScode.

---------

24/11/20

We switched developement from vs 2019 to vscode, with GNU 8.1.0,
we can now use a newer Version of OPEN MP including atomics

The next step is to code a parallel boruvka.

---------

---------

26/11/20
I parallelized the single loop in Boruvkastep without any careful adjustements.
I ran a simple test and took the mean runtime, this to my surprise I got a nice speedup for parallel implementation.

This could also be because of other things than parallelization.

---------

25/11/20

I set up the files for Boruvka parallel and environment for my laptop.

Self note:
Apparently we need a settings.json file for the intellisense to work in VScode.

---------

24/11/20

We switched developement from vs 2019 to vscode, with GNU 8.1.0,
we can now use a newer Version of OPEN MP including atomics

The next step is to code a parallel boruvka.

---------
