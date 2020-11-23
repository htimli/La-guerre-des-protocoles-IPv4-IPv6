#include "BiConResolution.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>

bool est_solution(BiConGraph *graph)
{
	for (int i = 0; i < orderG(graph->graph); i++)
	{
		for (int j = 0; j < orderG(graph->graph); j++)
		{
			if (isEdge(graph->graph, i, j))
			{
				if (getColor(graph->graph, i) != getColor(graph->graph, j &&
						(!(graph->translators[i]) || !(graph->translators[j]))))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void avancer_index(int index, int n, int *indexTab, bool *vertexBitMap)
{
	if (indexTab[index] == n - index - 1)
		avancer_index(index + 1, n, indexTab, vertexBitMap);
	vertexBitMap[indexTab[index]] = 0;
	indexTab[index]++;
	vertexBitMap[indexTab[index]] = 1;
}

bool prochaine_combinaison(int cb_size, int n, int *indexTab, bool *vertexBitMap)
{
	bool end = true;
	for (int i = n - 1; i >= n - cb_size; i++)
	{
		if (!vertexBitMap[i])
		{
			end = false;
			break;
		}
	}
	if (end)
		return false;
	avancer_index(0, n, indexTab, vertexBitMap);
	return true;
}
void ajouter_convertisseur(BiConGraph *graph, int n, bool *vertexBitMap)
{
	for (int j = 0; j < n; j++)
	{
		if (vertexBitMap[j])
		{
			addTranslator(graph, j);
			return;
		}
	}
}

bool BruteForceBiCon(BiConGraph *G)
{
	int n = orderG(G->graph);

	bool vertexBitMap[n];
	int indexTab[n / 2 + 1];

	for (int i = 0; i < n; i++)
		vertexBitMap[i] = false;
	for (int i = 0; i < n / 2 + 1; i++)
		indexTab[i] = 0;
	for (int k = 1; k <= n / 2; k++)
	{
		for (int cb_size = 1; cb_size <= k; cb_size++)
		{
			for (int i = 0; i < cb_size; i++)
			{
				vertexBitMap[i] = 1;
				indexTab[i] = 0;
			}
			do
			{
				ajouter_convertisseur(G, n, vertexBitMap);
				if (est_solution(G))
					return true;
				resetTranslator(G);
			} while (prochaine_combinaison(cb_size, n, indexTab, vertexBitMap));
			
			for (int i = 0; i < n; i++)
				vertexBitMap[i] = false;
		}
	}
	return false;
}

int computesCC(BiConGraph graph)
{
	Graph g = graph.graph;

	int nb_CC = graph.numComponents;
	for (int i = 0; i < g.numNodes; i++)
	{
		for (int j = 0; j < g.numNodes; j++)
		{
			if (isEdgeHomogeneous(graph, i, j))
				nb_CC--;
		}
		break;
	}
	return nb_CC;
}

bool GreedyBiCon(BiConGraph *graph)
{

	Graph g = graph->graph;
	int n = g.numNodes;

	BiConGraph Bg = initializeGraph(g);

	int nb_CC = Bg.numComponents;
	printf("nb_CC = %d start\n", nb_CC);

	bool acc;

	acc = true;
	for (int i = 0; i < n; i++)
	{
		while (nb_CC > 1)
		{
			for (int j = 0; j < n; j++)
			{
				if (isEdgeHomogeneous(Bg, i, j))
				{
					Bg.translators[i] = true;

					if (est_solution(&Bg))
						return true;

					nb_CC--;
					printf("nb_CC = %d\n", nb_CC);
					break;
				}
			}
			break;
		}
	}

	return false;
}
