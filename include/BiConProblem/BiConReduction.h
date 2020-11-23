/**
 * @file BiConReduction.h
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief An implementation of the Coca project for 2020 year. Converts a 2-coloured graph g and an integer p to a formula true if and only if it exists a translator set of size p.
 * Provides functions to generate the formula and the necessary variables, alongside function to decode a solution from a model of the formula.
 * @version 1
 * @date 2020-10-22
 * 
 * @copyright Creative Commons
 * 
 */

#ifndef BICON_RED_H_
#define BICON_RED_H_

#include "Graph.h"
#include "BiConGraph.h"
#include <z3.h>


/**
 * @brief Get the Variable representing that the node is the @p number translator.
 * 
 * @param ctx The solver context.
 * @param node The node identifier.
 * @param number The index of the translator.
 * @return Z3_ast A formula representing y_{@p node,@p number}
 */
Z3_ast getVariableIsIthTranslator(Z3_context ctx, int node, int number);

/**
 * @brief Get the Variable p_{@p child,@p parent} representing that the connected component @p parent is the parent of the connected component @p child in the reduction.
 * 
 * @param ctx The solver context.
 * @param child The number of a connected component.
 * @param parent The number of a connected component.
 * @return Z3_ast A formula representing p_{@p child,@p parent}.
 */
Z3_ast getVariableParent(Z3_context ctx, int child, int parent);

/**
 * @brief Get the Variable l_{@p component,@p level} representing that the connected component @p component is at level @p level in the spanning tree.
 * 
 * @param ctx The solver context.
 * @param level The level.
 * @param component The number of the component.
 * @return Z3_ast A formula representing l_{@p component,@p level}
 */
Z3_ast getVariableLevelInSpanningTree(Z3_context ctx, int level, int component);


/**
 * @brief Generates a SAT formula satisfiable if and only if there is a set of translators of size @p size such that the graph admits a valid path between any two nodes.
 * 
 * @param ctx The solver context.
 * @param graph A BiConGraph.
 * @param size The size of the translator set.
 * @return Z3_ast The formula.
 * @pre graph must be an initialized BiConGraph with computed connected components.
 */
Z3_ast BiConReduction(Z3_context ctx, BiConGraph graph, int size);

/**
 * @brief Gets the translator set from a model and adds it to the BiConGraph.
 * 
 * @param ctx The solver context.
 * @param model A variable assignment.
 * @param graph A BiConGraph.
 * @param size The size of the translator set.
 * 
 * @pre @p model must be a valid model.
 * @pre @p graph must be a valid BiConGraph.
 */
void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, BiConGraph *graph, int size);

#endif