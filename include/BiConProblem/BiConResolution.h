/**
 * @file BiConResolution.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  Algorithms to solve directly the BiCon problem
 * @version 
 * @date 2020-10-22
 * 
 * @copyright Creative Commons.
 * 
 */

#ifndef COCA_BICONRESOLUTION_H
#define COCA_BICONRESOLUTION_H

#include "BiConGraph.h"

/**
 * @brief Brute Force Algorithm. If there is a result, the solution will be stored in graph.translator. If no solution, graph won't be modified.
 * 
 * @param graph An instance of the problem.
 * @return true if there is a solution.
 * @return false if there is no solution.
 * 
 * @pre graph must be valid.
 */
bool BruteForceBiCon(BiConGraph *graph);

/**
 * @brief Greedy algorithm. If there is a result, the solution will be stored in graph.translator. If no solution, graph won't be modified.
 * 
 * @param graph An instance of the problem.
 * @return true if there is a solution.
 * @return false if there is no solution.
 * 
 * @pre graph must be valid.
 */
bool GreedyBiCon(BiConGraph *graph);

#endif