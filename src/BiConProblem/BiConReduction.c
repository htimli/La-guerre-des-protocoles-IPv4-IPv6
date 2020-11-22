
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



    /***************************/
    /********** Phi_a **********/
    /***************************/

    //***** Phi_a1
    Z3_ast each_u[n];
    for (int u=0; u<n; u++) {
        Z3_ast each_v[n-1];
        int shift = 0;
        for (int v=0; v<n; v++) {
            if (v == u)
                shift = 1;
            else {
                Z3_ast each_tranlator[k];
                for (int i=0; i<k; i++) {
                    Z3_ast x_ui = Lit_x_ui[u][i];
                    Z3_ast x_vi = Lit_x_ui[v][i];
                    Z3_ast neg_u_v[2] = {Z3_mk_not(ctx, x_ui), Z3_mk_not(ctx, x_vi)};
                    Z3_ast disj = Z3_mk_or(ctx, 2, neg_u_v);    // => not(x_ui) or not(x_vi)
                    each_tranlator[i] = disj;
                }
                Z3_ast conj_translators = Z3_mk_and(ctx, k, each_tranlator); // AND on each translator
                each_v[v - shift] = conj_translators;
            }
        }
        each_u[u] = Z3_mk_and(ctx, n-1, each_v);  // for each vertex u : AND on each vertex v
    }
    Z3_ast phi_a1 = Z3_mk_and(ctx, n, each_u);  // => AND on each vertex u
    
    //***** Phi_a2
    //Z3_ast each_u[n];
    for(int u=0; u<n; u++) {
        Z3_ast each_translator_i1[k];
        for(int i1=0; i1<k; i1++) {
            Z3_ast each_translator_i2[k-1];
            int shift = 0;
            for(int i2=0; i2<k; i2++) {
                if (i1 == i2)
                    shift = 1;
                else {
                    Z3_ast x_ui1 = Lit_x_ui[u][i1];
                    Z3_ast x_ui2 = Lit_x_ui[u][i2];
                    Z3_ast neg_ui1_ui2[2] = {Z3_mk_not(ctx, x_ui1), Z3_mk_not(ctx, x_ui2)};
                    Z3_ast disj = Z3_mk_or(ctx, 2, neg_ui1_ui2);
                    each_translator_i2[i2 - shift] = disj;
                }
            }
            Z3_ast conj_translators = Z3_mk_and(ctx, k-1, each_translator_i2);
            each_translator_i1[i1] = conj_translators;
        }
        each_u[u] =  Z3_mk_and(ctx, k, each_translator_i1);
    }
    Z3_ast phi_a2 = Z3_mk_and(ctx, n, each_u); 
    
    //***** Phi_a
    Z3_ast phi_a_composantes[2] = {phi_a1, phi_a2};
    Z3_ast phi_a = Z3_mk_and(ctx, 2, phi_a_composantes);
    


    /********************************/
    /********** Phi_racine **********/
    /********************************/

    //***** Phi_r2



















    return Z3_mk_true(ctx); 
}


Z3_ast compute_phi_r2(Z3_context ctx, int j1, Z3_ast **Lit_l_jh, int maxJ) {
    Z3_ast l_j10 = Lit_l_jh[j1][0];

    Z3_ast each_neg[maxJ-1];
    int shift = 0;
    for (int j2=0; j2<maxJ; j2++) {
        if (j1 == j2)
            shift = 1;
        else {
            Z3_ast l_j20 = Lit_l_jh[j2][0];
            each_neg[j2 - shift] = Z3_mk_not(ctx, l_j20);
        }
    }
    Z3_ast each_CC = Z3_mk_and(ctx, maxJ-1, each_neg);

    Z3_ast conj[2] = {l_j10, each_CC};
    Z3_ast phi_r2 = Z3_mk_and(ctx, 2, conj);
    return phi_r2;
}
Z3_ast compute_phi_r1(Z3_context ctx, int j, Z3_ast **Lit_p_j1j2, int maxJ) {

        Z3_ast each_y[maxJ];
        Z3_ast jz_and_not_jy[maxJ];
        for(int y;y<maxJ-1;y++){
            if(j!=y){
                Z3_ast p_j_y = Lit_p_j1j2[j][y];
                Z3_ast each_z[maxJ];
                for(int z;z<maxJ-1;z++)               
                    each_z[z]= Lit_p_j1j2[j][z];                                   
                Z3_ast or_j_z=Z3_mk_or(ctx, maxJ,each_z);
                Z3_ast tmp[2] ={Z3_mk_not(ctx,p_j_y),or_j_z}; 
                jz_and_not_jy[y]=Z3_mk_and(ctx,2,tmp);
            }
            each_y[y] = Z3_mk_and(ctx, maxJ,jz_and_not_jy);   
        }
        return Z3_mk_and(ctx,maxJ,each_y);      
}

Z3_ast compute_phi_b(Z3_context ctx, int j, Z3_ast **Lit_p_j1j2, int maxJ) {
    //Example phi_b
    //Z3_ast Lit_p_j1j2[maxJ][maxJ];

        Z3_ast each_y[maxJ];
        Z3_ast jy_and_not_jz[maxJ];
        for(int y;y<maxJ-1;y++){
            if(j!=y){
                Z3_ast p_j_y = Lit_p_j1j2[j][y];
                Z3_ast each_z[maxJ];
                for(int z;z<maxJ-1;z++){                   
                    Z3_ast p_j_z = Lit_p_j1j2[j][z];
                    each_z[z]=Z3_mk_not(ctx,p_j_z );
                }
                Z3_ast neg_j_z=Z3_mk_and(ctx, maxJ,each_z);
                Z3_ast tmp[2] ={p_j_y,neg_j_z}; 
                jy_and_not_jz[y]=Z3_mk_and(ctx,2,tmp);
            }
            each_y[y] = Z3_mk_or(ctx, maxJ,jy_and_not_jz);   
        }
        Z3_ast phi_b = Z3_mk_and(ctx,maxJ,each_y);
        return phi_b;
}

void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, BiConGraph *graph, int size){
    return;
}

