#include "BiConResolution.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>


bool est_solution(BiConGraph *graph)
{
	for(int i=0; i< orderG(graph->graph);i++){
		for(int j=0; j< orderG(graph->graph);j++){
			if(isEdge(graph->graph,i,j)){
				if(getColor(graph->graph,i)!=getColor(graph->graph,j &&
				(!(graph->translators[i]) || !(graph->translators[j])))){
					return false ;
				}	
			}	
		}
	}
	return true ;
}

void avancer_index(int index,int n,int * indexTab,bool*vertexBitMap )
{
	if(indexTab[index] == n-index-1)	
		avancer_index(index+1,n,indexTab,vertexBitMap);
	vertexBitMap[indexTab[index]]=0;
	indexTab[index]++;
	vertexBitMap[indexTab[index]]=1;
}	 			

bool prochaine_combinaison(int cb_size,int n,int * indexTab,bool* vertexBitMap)
{
	bool end = true ;
	for(int i=n-1 ; i>=n-cb_size ; i++){
		if(!vertexBitMap[i]){
			end = false ;
			break ;
		}
	}	
	if(end)
		return false ;
	avancer_index(0,n,indexTab,vertexBitMap);
	return true ;
}	 
void ajouter_convertisseur(BiConGraph *graph,int n, bool*vertexBitMap)
{
	for(int j=0;j<n;j++){
		if(vertexBitMap[j]){
			addTranslator(graph,j);
			return;
		}	
	}	
}


bool BruteForceBiCon(BiConGraph *G)
{	
	int n = orderG(G->graph);

	bool vertexBitMap[n];
	int indexTab[n/2+1];
	
	for(int i =0 ; i<n ; i++)
		vertexBitMap[i]=false;
	for(int i =0 ; i<n/2+1;i++)
		indexTab[i]=0;
		
	for(int k=1 ; k<=n/2 ; k++){
		for(int cb_size=1 ;cb_size<=k; k++){
		
			for(int i=0;i<cb_size;i++){
				vertexBitMap[i]=1;
				indexTab[i]=0;
			}
			do {
				ajouter_convertisseur(G,n,vertexBitMap);
				if(est_solution(G))
					return true;
				resetTranslator(G);		
				} while(prochaine_combinaison(cb_size,n,indexTab,vertexBitMap));
			for(int i =0 ; i<n ; i++)
				vertexBitMap[i]=false;	
		} 
	}
    return false;
}
bool GreedyBiCon(BiConGraph *graph){
    printf("Brute Force not implemented\n");
    return false;
}
