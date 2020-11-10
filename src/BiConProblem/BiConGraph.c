#include "BiConGraph.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

BiConGraph initializeGraph(Graph graph){
    BiConGraph result;
    result.graph = graph;
    result.heterogeneousEdges = (bool*) malloc(graph.numNodes*graph.numNodes*sizeof(bool));
    result.homogeneousComponents = (bool**) malloc(graph.numNodes*sizeof(bool*));
    for(int i = 0; i < graph.numNodes; i++) result.homogeneousComponents[i] = (bool*) malloc(graph.numNodes*sizeof(bool));
    result.translators = (bool*) malloc(graph.numNodes*sizeof(bool));
    result.numComponents = 0;


    for(int i = 0; i < graph.numNodes; i++){
        for(int j = 0; j < graph.numNodes; j++){
            result.heterogeneousEdges[i*graph.numNodes+j] = graph.edges[i*graph.numNodes+j] && (getColor(graph,i) != getColor(graph,j));
        }
    }

    for(int i = 0; i < graph.numNodes; i++) result.translators[i] = false;

    computesHomogeneousComponents(&result);

    return result;
}

void resetTranslator(BiConGraph *graph){
    graph->numComponents = 0;
    for(int i = 0; i < graph->graph.numNodes; i++){
        for(int j = 0; j < graph->graph.numNodes; j++){
            graph->heterogeneousEdges[i*graph->graph.numNodes+j] = graph->graph.edges[i*graph->graph.numNodes+j] && (getColor(graph->graph,i) != getColor(graph->graph,j));
        }
    }

    for(int i = 0; i < graph->graph.numNodes; i++) graph->translators[i] = false;

    computesHomogeneousComponents(graph);
}

void deleteBiConGraph(BiConGraph graph){
    free(graph.heterogeneousEdges);
    free(graph.translators);
    for(int i = 0; i < graph.graph.numNodes; i++) free(graph.homogeneousComponents[i]);
    free(graph.homogeneousComponents);
}

void setHeterogeneousEdge(BiConGraph* graph,int node1, int node2){
    if(isEdge(graph->graph,node1,node2)){
        graph->heterogeneousEdges[node1*graph->graph.numNodes+node2] = true;
        graph->heterogeneousEdges[node2*graph->graph.numNodes+node1] = true;
    }
}

void SetHomogeneousEdge(BiConGraph* graph, int node1, int node2){
    if(isEdge(graph->graph,node1,node2)){
        graph->heterogeneousEdges[node1*graph->graph.numNodes+node2] = false;
        graph->heterogeneousEdges[node2*graph->graph.numNodes+node1] = false;
    }
}

bool isEdgeHomogeneous(BiConGraph graph, int node1, int node2){
    return(isEdge(graph.graph,node1,node2) && !graph.heterogeneousEdges[node1*graph.graph.numNodes+node2]);
}

bool isEdgeHeterogeneous(BiConGraph graph, int node1, int node2){
    return(isEdge(graph.graph,node1,node2) && graph.heterogeneousEdges[node1*graph.graph.numNodes+node2]);
}

void addTranslator(BiConGraph *graph, int node){
    graph->translators[node] = true;
    for(int i = 0; i < graph->graph.numNodes; i++) {
        graph->heterogeneousEdges[i*graph->graph.numNodes+node] = false;
        graph->heterogeneousEdges[node*graph->graph.numNodes+i] = false;
    }

    computesHomogeneousComponents(graph);
}

void computesComponent(BiConGraph* graph, int node, int component){
    if(graph->homogeneousComponents[component][node]) return;
    graph->homogeneousComponents[component][node] = true;
    for(int i = 0; i < graph->graph.numNodes; i++){
        if(isEdgeHomogeneous(*graph,node,i)) computesComponent(graph,i,component);
    }
    return;
}

void computesHomogeneousComponents(BiConGraph* graph){
    graph->numComponents = 0;
    int currentNode = 0;
    while(currentNode < orderG(graph->graph)){
        for(int i = 0; i< orderG(graph->graph); i++) graph->homogeneousComponents[graph->numComponents][i] = false;
        computesComponent(graph,currentNode,graph->numComponents);
        bool acc = true;
        while(currentNode < orderG(graph->graph) && acc){
            acc = false;
            for(int i = 0; i <= graph->numComponents; i++){
                acc = acc || graph->homogeneousComponents[i][currentNode];
            }
            if(acc) currentNode++;
        }
        (graph->numComponents)++;
    }
}

bool areInSameComponent(BiConGraph graph, int node1, int node2){
    bool result = false;
    for(int i = 0; i < graph.numComponents; i++){
        result = result || (graph.homogeneousComponents[i][node1] && graph.homogeneousComponents[i][node2]);
    }
    return result;
}


void printTranslator(BiConGraph graph){
    if(graph.numComponents !=1) printf("Warning, printing a set of translator that yields %d homogeneous components\n",graph.numComponents);
    printf("Translator nodes: ");
    for(int i = 0; i < graph.graph.numNodes; i++){
        if(graph.translators[i]) printf("%s(%d), ",getNodeName(graph.graph,i),i);
    }
    printf("\n");
}

void createDotOfBiConGraph(BiConGraph graph, char* name){
    if(graph.numComponents !=1) printf("Warning, printing a set of translator that yields %d homogeneous components\n",graph.numComponents);

    FILE* file;

    struct stat st = {0};
    if(stat("./sol", &st) == -1) mkdir("./sol",0777);

    if(name == NULL){
        char nameFile[30];
        snprintf(nameFile,30,"sol/result.dot");
        file = fopen(nameFile,"w");
        fprintf(file,"graph Sol{\n");
    }
    else{
        int length = strlen(name) +12;
        char nameFile[length];
        snprintf(nameFile,length,"sol/%s.dot",name);
        file = fopen(nameFile,"w");
        fprintf(file,"graph %s{\n",name);
    }

    for(int node = 0; node<orderG(graph.graph); node ++){
        fprintf(file,"%s",getNodeName(graph.graph,node));
        if(strcmp(getColorString(graph.graph,getColor(graph.graph,node)),"")!=0){
            fprintf(file,"[color=%s]",getColorString(graph.graph,getColor(graph.graph,node)));
        }
        if(graph.translators[node]) fprintf(file,"[shape=square]");
        fprintf(file,";\n");
    }

    for(int node = 0; node < orderG(graph.graph); node++){
        for(int node2 = 0; node2 < node; node2++){
            if(isEdge(graph.graph,node,node2)) fprintf(file,"%s -- %s;\n",getNodeName(graph.graph,node),getNodeName(graph.graph,node2));
        }
    }

    fprintf(file,"}\n");

    fclose(file);

}