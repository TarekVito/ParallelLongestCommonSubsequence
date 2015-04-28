# ParallelLongestCommonSubsequence
A C++ parallel implementation for Longest Common Subsequence (LCS) problem.
The project contains a DP algorithm for solving LCS problem in addition a parallelized version of the algorithm.
The main idea is that the cells (states) that are at the same anti-diagonal in the DP table can be computed independently.

More details can be found here :
http://www.thinkmind.org/download.php?articleid=infocomp_2011_7_20_10120

