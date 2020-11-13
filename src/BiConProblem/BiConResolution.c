#include "BiConResolution.h"
#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>


bool est_solution(BiConGraph *graph){
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

bool BruteForceBiCon(BiConGraph *graph){
	int count =1;
	while(count < orderG(graph->graph)/2){
		for(int cb=1;cb<count;cb++){ //TODO :pour_chaque_combinaison 
			addTranslator(graph,cb);
			for(int node=1; node<cb;node++){
				addTranslator(graph,node);
					if(!est_solution(graph)){
						 resetTranslator(graph);
					} 	
					else {
						return true ;
					}					
			}
			if(!est_solution(graph)){
				resetTranslator(graph);
			} 		
		}
	}
	
    printf("Brute Force false \n");
    return false;

}
bool GreedyBiCon(BiConGraph *graph){
    printf("Brute Force not implemented\n");
    return false;
}
