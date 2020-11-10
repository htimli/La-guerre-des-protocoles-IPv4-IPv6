/**
 * @file main.c
 * @author Vincent Penelle (vincent.penelle@u-bordeaux.fr)
 * @brief  File putting the whole program together.
 * @version 1
 * @date 2019-07-22
 * 
 * @copyright Creative Commons.
 * 
 */

#include "Graph.h"
#include "Parsing.h"
#include "BiConReduction.h"
#include "Z3Tools.h"
#include "Parser.h"
#include "BiConGraph.h"
#include "BiConResolution.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void usage(){
    printf("Use: graphProblemSolver [options] file\n");
    printf(" file should contain a colored graph in dot format.\n The program decides if there exists a translator set for the graph in input.\n Can apply a brute force algorithm, a greedy algorithm or a reduction to SAT. In the latter case, a size of the expected solution must be given.\n Can display the result both on the command line or in dot format.\n");
    printf("Options: \n");
    printf(" -h         Displays this help\n");
    printf(" -v         Activate verbose mode (displays parsed graphs)\n");
    printf(" -B         Solves the problem using the brute force algorithm\n");
    printf(" -G         Solves the problem using the greedy algorithm\n");
    printf(" -R SIZE    Solves the problem using a reduction and searches for a solution of size SIZE\n");
    printf(" -F         Displays the formula computed . Only active if -R is active\n");
    printf(" -t         Displays the translator set found [if not present, only displays the existence of the set].\n");
    printf(" -f         Writes the result with colors in a .dot file. See next option for the name. These files will be produced in the folder 'sol'.\n");
    printf(" -o NAME    Writes the output graph in \"NAME_Brute.dot\" or \"NAME_SAT.dot\" depending of the algorithm used and the formula in \"NAME.formul\". [if not present: \"result_SAT.dot\", \"result_Brute.dot\" and \"result.formul\"]\n");
}

int main(int argc, char *argv[])
{
    if(argc<2) {
        usage();
        return 0;
    }

    bool verbose = false;
    bool displayTerminal = false;
    bool outputFile = false;
    bool printformula = false;
    bool bruteForce = false;
    bool reduction = false;
    bool greedy = false;
    int size = 0;
    char* solutionName = "default";
    char* realArgs[argc];
    int numArgs = 0;

    int option;

    while((option = getopt(argc, argv, ":hvFBGR:tfo:")) != -1){
      switch(option){
         case 'h':
            usage();
            return EXIT_SUCCESS;
         case 'v':
            verbose = true;
            break;
         case 'B':
             bruteForce = true;
             break;
         case 'G':
             greedy = true;
             break;
         case 'R':
            reduction = true;
            size = atoi(optarg);
            break;
         case 'F':
             //printf("Don't insist, I'm not showing you the solution of the assignment yet!\n");
             printformula = true;
             break;
         case 't':
             displayTerminal = true;
             break;
         case 'f':
             outputFile = true;
             break;
         case 'o':
             solutionName = optarg;
             break;
         case '?':
            printf("unknown option: %c\n", optopt);
            break;
      }
    }

    if(argc - optind < 1) { printf("No argument given. Exiting.\n"); return 0;}

    Graph graph = getGraphFromFile(argv[optind]);

    if(verbose) printGraph(graph);

    BiConGraph biGraph = initializeGraph(graph);

    if(verbose) {
        printf("Connected Components: \n");
        for(int i = 0; i<biGraph.numComponents; i++){
            for(int j = 0; j < orderG(graph); j++){
                if(biGraph.homogeneousComponents[i][j]) printf("- %s ",getNodeName(graph,j));
            }
            printf("\n");
        }
    }

    if(bruteForce){
        printf("\n*******************\n*** Brute Force ***\n*******************\n\n");
        clock_t start = clock();
        bool res = BruteForceBiCon(&biGraph);
        double end = (double) (clock() - start)/CLOCKS_PER_SEC;
        if(res){
            int sizeBrute = 0;
            for(int i = 0; i < biGraph.graph.numNodes; i++) sizeBrute+=biGraph.translators[i];
            printf("Solution of size %d found by Brute Force in %g seconds\n",sizeBrute,end);
            if(displayTerminal) printTranslator(biGraph);
            int length = strlen(solutionName) +12;
            char nameFile[length];
            snprintf(nameFile,length,"%s_Brute",solutionName);
            if(outputFile) {
                createDotOfBiConGraph(biGraph,nameFile);
                printf("Solution printed in sol/%s.dot.\n",nameFile);
            }
        }
        else printf("No solution found by Brute Force in %g seconds\n",end);
        resetTranslator(&biGraph);
    }

    if(greedy){
        printf("\n************************\n*** Greedy Algorithm ***\n************************\n\n");
        clock_t start = clock();
        bool res = GreedyBiCon(&biGraph);
        double end = (double) (clock() - start)/CLOCKS_PER_SEC;
        if(res){
            int setSize = 0;
            for(int i = 0; i < graph.numNodes; i++) setSize += biGraph.translators[i];
            printf("Solution of size %d found by Greedy in %g seconds\n",setSize,end);
            if(displayTerminal) printTranslator(biGraph);
            int length = strlen(solutionName) +12;
            char nameFile[length];
            snprintf(nameFile,length,"%s_Greedy",solutionName);
            if(outputFile) {
                createDotOfBiConGraph(biGraph,nameFile);
                printf("Solution printed in sol/%s.dot.\n",nameFile);
            }
        }
        else printf("No solution found by Greedy in %g seconds\n",end);
        resetTranslator(&biGraph);
    }


    if(reduction){
        printf("\n************************\n*** Reduction to SAT ***\n************************\n\n");

        if(size <= 0){
            printf("No size given, or size given less than 0, I refuse to compute the formula for it!\n");
        }
        else {
            Z3_context ctx = makeContext();

            clock_t start = clock();

            Z3_ast formula;
            formula = BiConReduction(ctx,biGraph,size);

            clock_t timeFormula = clock();

            printf("formula computed in %g seconds\n",(double) (timeFormula -   start)/CLOCKS_PER_SEC);

            if(printformula) {
                struct stat st = {0};
                if(stat("./sol", &st) == -1) mkdir("./sol",0777);
                int length = strlen(solutionName) +12;
                char nameFile[length];
                snprintf(nameFile,length,"sol/%s.formula",solutionName);
                FILE *file = fopen(nameFile,"w");
                fprintf(file,"formule : %s\n",Z3_ast_to_string(ctx,formula));
                fclose(file);
                printf("Formula printed\n");
            }

            Z3_model model;
            Z3_lbool isSat = solveFormula(ctx,formula,&model);

            clock_t timeSat = clock();

            printf("solution computed in %g seconds\n",(double) (timeSat -  timeFormula)/CLOCKS_PER_SEC);

            switch(isSat){
                case Z3_L_FALSE:
                    printf("No translator set of size %d exists.\n",size);
                    break;

                case Z3_L_UNDEF:
                    printf("Not able to decide if there is a translator set     of size %d.\n",size);
                    break;

                case Z3_L_TRUE:
                    printf("There is a translator set of size %d.\n",size);

                    if(displayTerminal) {
                        getTranslatorSetFromModel(ctx,model,&biGraph,   size);
                        printTranslator(biGraph);
                    }

                    if(outputFile) {
                        getTranslatorSetFromModel(ctx,model,&biGraph,   size);
                        int length = strlen(solutionName) +12;
                        char nameFile[length];
                        snprintf(nameFile,length,"%s_Sat",solutionName);
                        createDotOfBiConGraph(biGraph,nameFile);
                        printf("Solution printed in sol/%s.dot.\n",nameFile);
                    }

                    break;
            }

            Z3_del_context(ctx);
        }
    }

    deleteBiConGraph(biGraph);

    deleteGraph(graph);

    return 0;
}
