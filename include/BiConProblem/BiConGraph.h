/**
 * @file BiConGraph.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  Structure to store a graph with heterogeneous edges informations for the BiCon Problem
 * @version 
 * @date 2020-10-22
 * 
 * @copyright Creative Commons.
 * 
 */

#ifndef COCA_BICONGRAPH_H
#define COCA_BICONGRAPH_H

#include <stdbool.h>
#include "Graph.h"

/**
 * @brief The struct containing a graph, its heteregeneous edges, homogenous components, and current translator set.
 */
typedef struct{
    Graph graph; ///< The graph.
    bool* heterogeneousEdges; ///< The edges that are heterogeneous (taking into account the translators).
    bool** homogeneousComponents; ///< The homogeneous connected components. Each member is an array of size @p graph.numNodes that sets all nodes in a connected component to true and other to false. There are @p numComponents such arrays, and a node is true in exactly one array.
    bool* translators; ///< The translator nodes.
    int numComponents; ///< The number of homogeneous components.
} BiConGraph;

/**
 * @brief Initializes the struct from a graph. Does NOT creates a copy of the graph. Starts with empty translators.
 * 
 * @param graph A graph
 * @return BiConGraph The corresponding BiConGraph with no translator.
 * @pre @p graph must be a valid graph.
 */
BiConGraph initializeGraph(Graph graph);

/**
 * @brief Resets the translator set to empty, and recomputes heterogeneous edges and connected components.
 * 
 * @param graph A BiConGraph.
 * @pre @p graph must be valid.
 */
void resetTranslator(BiConGraph *graph);

/**
 * @brief Frees the additional informations of a BiConGraph, but NOT the graph (cf initialize).
 * 
 * @param graph A BiConGraph.
 * 
 * @pre @p graph must be valid.
 */
void deleteBiConGraph(BiConGraph graph);

/**
 * @brief Set the edge (@p node1,@p node2) as heterogeneous (if it exists, otherwise does nothing).
 * 
 * @param graph A BiConGraph.
 * @param node1 A node.
 * @param node2 A node.
 * 
 * @pre @p graph must be valid.
 * @pre @p node1 < @p graph->graph.numNodes.
 * @pre @p node2 < @p graph->graph.numNodes.
 */
void setHeterogeneousEdge(BiConGraph* graph,int node1, int node2);

/**
 * @brief Set the edge (@p node1,@p node2) as heterogeneous (if it exists, otherwise does nothing).
 * 
 * @param graph A BiConGraph.
 * @param node1 A node.
 * @param node2 A node.
 * 
 * @pre @p graph must be valid.
 * @pre @p node1 < @p graph->graph.numNodes.
 * @pre @p node2 < @p graph->graph.numNodes.
 */
void SetHomogeneousEdge(BiConGraph* graph, int node1, int node2);

/**
 * @brief
 * 
 * @param graph A BiConGraph.
 * @param node1 A node.
 * @param node2 A node.
 * @return True if (@p node1, @p node2) is an homogeneous edge.
 * 
 * @pre @p graph must be valid.
 * @pre @p node1 < @p graph.graph.numNodes.
 * @pre @p node2 < @p graph.graph.numNodes.
 */
bool isEdgeHomogeneous(BiConGraph graph, int node1, int node2);

/**
 * @brief
 * 
 * @param graph A BiConGraph.
 * @param node1 A node.
 * @param node2 A node.
 * @return True if (@p node1, @p node2) is an heterogeneous edge.
 * 
 * @pre @p graph must be valid.
 * @pre @p node1 < @p graph.graph.numNodes.
 * @pre @p node2 < @p graph.graph.numNodes.
 */
bool isEdgeHeterogeneous(BiConGraph graph, int node1, int node2);

/**
 * @brief Sets nodes to be a translator, and recomputes homogeneous components.
 * 
 * @param graph A BiConGraph.
 * @param node A node.
 * 
 * @pre graph must be valid.
 * @pre node < graph.graph.numNodes.
 */
void addTranslator(BiConGraph* graph, int node);

/**
 * @brief Computes the homogeneous components.
 * 
 * @param graph a BiConGraph.
 * 
 * @pre graph must be a valid BiConGraph.
 */
void computesHomogeneousComponents(BiConGraph* graph);

/**
 * @brief Tests if @p node1 and @p node2 are in the same homogeneous component.
 * 
 * @param graph A BiConGraph.
 * @param node1 A node.
 * @param node2 A node.
 * @return true If @p node1 and @p node2 are in the same homogeneous component.
 * @return false Otherwise.
 * 
 * @pre @p graph must be a valid BiConGraph
 * @pre @p node1 < @p graph.graph.numNodes
 * @pre @p node2 < @p graph.graph.numNodes
 */
bool areInSameComponent(BiConGraph graph, int node1, int node2);

/**
 * @brief Displays the translator set of the BiConGraph.
 * 
 * @param graph A BiConGraph.
 * @pre @p graph must be a valid BiConGraph.
 */
void printTranslator(BiConGraph graph);

/**
 * @brief Creates the file ("%s.dot",name) representing the solution to the problem described by @p set, or ("result.dot") if name is NULL.
 * 
 * @param graph A graph.
 * @param name The name of the output file.
 * @pre @p graph must be a valid graph.
 */
void createDotOfBiConGraph(BiConGraph graph, char* name);

#endif