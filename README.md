# MST_Parallel

This is a Repo that stores all files related to any work regarding Parallel Algorithms and Graph creation.



Work-Log

---------

26/11/20
I parallelized the single loop in Boruvkastep without any careful adjustements.
I ran a simple test and took the mean runtime, this to my surprise I got a nice speedup for parallel implementation.
Since this was not a "real" runtime benchmark I won't publish any real code. I will however safe the results in the TestResult.txt file for later comparison.

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
