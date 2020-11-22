
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
    int n = orderG(biGraph.graph);
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
    //phi_1,b
    //Z3_ast each_u[n];
    for(int u=0;u<n;u++){
        Z3_ast each_translator_1[size];
        for(int i=1;i<=size;i++){
            Z3_ast each_translator_2[size];
            int shift=0;
            for(int j=1;j<=size;j++){
                if(i==j)
                    shift=1;
                else{
                    Z3_ast x_ui = Lit_x_ui[u][i];
                    Z3_ast x_uj = Lit_x_ui[u][j];
                    Z3_ast neg_ui_uj[2]={Z3_mk_not(ctx,x_ui),Z3_mk_not(ctx,x_uj)};
                    Z3_ast disj = Z3_mk_or(ctx, 2, neg_ui_uj);
                    each_translator_2[i]=disj; 
                }          
            }
            Z3_ast conj_translators =Z3_mk_and(ctx, size, each_translator_2);
            each_translator_1[size-shift]=conj_translators;
        }
        each_u[u] =  Z3_mk_and(ctx, n-1, each_translator_1);
    }
    Z3_ast phi_1b = Z3_mk_and(ctx, n, each_u); 
    
    //phi_a
    Z3_ast phi_1a_1b[2]={phi_1b,phi_1a};
    Z3_ast phi_a = Z3_mk_and(ctx,2,phi_1a_1b);
    
    //phi_r2
    Z3_ast Lit_l_j0;
    for(int j=1; j<maxJ;j++){
        if (isFormulaSat(ctx,Lit_l_jh[j][0]))
            Lit_l_j0 = Lit_l_jh[j][0];
    }
    Z3_ast each_CC[maxJ];
    for(int y=1; y<maxJ;y++){
        if(Lit_l_j0 != Lit_l_jh[y][0]){
            Z3_ast neg_Lit_l_jh=Z3_mk_not(ctx,Lit_l_jh[y][0]);
             each_CC[y] = neg_Lit_l_jh;
        }     
    }
    Z3_ast conj =Z3_mk_and(ctx, maxJ-1, each_CC);
    
    
    Z3_ast tmp[2]={Lit_l_j0,conj};

    Z3_ast phi_r2 = Z3_mk_and(ctx,2,tmp);


















    return Z3_mk_true(ctx); 
}



void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, BiConGraph *graph, int size){
    return;
}

