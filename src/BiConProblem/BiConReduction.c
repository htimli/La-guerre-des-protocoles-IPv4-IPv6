
#include "BiConReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


Z3_ast getVariableIsIthTranslator(Z3_context ctx, int node, int number){
    char name[40];
    snprintf(name,40,"[%d,%d]",node,number);
    return mk_bool_var(ctx,name);
}

Z3_ast getVariableParent(Z3_context ctx, int child, int parent){
    char name[40];
    snprintf(name,40,"p_[%d,%d]",child,parent);
    return mk_bool_var(ctx,name);
}

Z3_ast getVariableLevelInSpanningTree(Z3_context ctx, int level, int component){
    char name[40];
    snprintf(name,40,"l_[%d,%d]",component,level);
    return mk_bool_var(ctx,name);
}

Z3_ast BiConReduction(Z3_context ctx, BiConGraph biGraph, int size){
    printf("Reduction not implemented\n");

    // Usefull values
    int n = order(biGraph.graph);
    int k = size;
    int maxJ = biGraph.numComponents;
    int maxH = maxJ;

    // Creating tabs with each literals once and for all
    Z3_ast Lit_x_ui[n][k];
    Z3_ast Lit_p_j1j2[maxJ][maxJ];
    Z3_ast Lit_l_jh[maxJ][maxH];
    
    for (int u=0; u<n; u++)
        for (int i=0; i<k; i++)
            Lit_x_ui[u][k] = getVariableIsIthTranslator(ctx, u, i);
    
    for (int j1=0; j1<maxJ; j1++)
        for (int j2=0; j2<maxJ; j2++)
            Lit_p_j1j2[j1][j2] = getVariableParent(ctx, j1, j2);

    for (int j=0; j<maxJ; j++)
        for (int h=0; h<maxH; h++)
            Lit_l_jh[j][h] = getVariableParent(ctx, h, j);


    // Example phi_1,a
    Z3_ast each_u[n];
    for (int u=0; u<n; u++) {
        Z3_ast each_v[n-1];
        for (int v=0; v<n; v++) {
            int shift = 0;
            if (v == u)
                shift = 1;
            else {
                Z3_ast each_tranlator[size];
                for (int i=1; i<=size; i++) {
                    Z3_ast x_ui = Lit_x_ui[u][i];
                    Z3_ast x_vi = Lit_x_ui[v][i];
                    Z3_ast neg_u_v[2] = {Z3_mk_not(ctx, x_ui), Z3_mk_not(ctx, x_vi)};
                    Z3_ast disj = Z3_mk_or(ctx, 2, neg_u_v);    // => (not(x_ui) or not(x_vi))
                    each_tranlator[i] = disj;
                }
                Z3_ast conj_translators = Z3_mk_and(ctx, size, each_tranlator); // => le grand ET sur tous les i
                each_v[v - shift] = conj_translators;
            }
        }
        each_u[u] = Z3_mk_and(ctx, n-1, each_v);  // => pour chaque u, le grand ET sur tous les v
    }
    Z3_ast phi_1a = Z3_mk_and(ctx, n, each_u);  // => le grand ET sur tous les u et v

    return Z3_mk_true(ctx); 
}

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, BiConGraph *graph, int size){
    return;
}

