
#include "BiConReduction.h"
#include "Z3Tools.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int n = 0;
int k = 0;
int maxJ = 0;
int maxH = 0;


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





/***************************/
/********** Phi_a **********/
/***************************/

//***** Phi_a1
Z3_ast compute_phi_a1(Z3_context ctx, Z3_ast Lit_x_ui[][k]) {
    Z3_ast each_u[n];
    for (int u=0; u<n; u++) {
        Z3_ast each_v[n-1];
        int shift = 0;
        for (int v=0; v<n; v++) {
            if (v == u)
                shift = 1;
            else {
                Z3_ast each_translator[k];
                for (int i=0; i<k; i++) {
                    Z3_ast x_ui = Lit_x_ui[u][i];
                    Z3_ast x_vi = Lit_x_ui[v][i];
                    Z3_ast neg_u_v[2] = {Z3_mk_not(ctx, x_ui), Z3_mk_not(ctx, x_vi)};
                    Z3_ast disj = Z3_mk_or(ctx, 2, neg_u_v);
                    each_translator[i] = disj;
                }
                Z3_ast conj_translators = Z3_mk_and(ctx, k, each_translator);
                each_v[v - shift] = conj_translators;
            }
        }
        each_u[u] = Z3_mk_and(ctx, n-1, each_v);
    }
    Z3_ast phi_a1 = Z3_mk_and(ctx, n, each_u);
    return phi_a1;
}

//***** Phi_a2
Z3_ast compute_phi_a2(Z3_context ctx, Z3_ast Lit_x_ui[][k]) {
    Z3_ast each_u[n];
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
    return phi_a2;
}

//***** Phi_a3
Z3_ast compute_phi_a3(Z3_context ctx, Z3_ast Lit_x_ui[][k]) {
    Z3_ast each_translator[k];
    for (int i=0; i<k; i++) {
        Z3_ast each_u[n];
        for (int u=0; u<n; u++) {
            each_u[u] = Lit_x_ui[u][i];
        }
        Z3_ast disj_x_ui = Z3_mk_or(ctx, n, each_u);
        each_translator[i] = disj_x_ui;
    }
    Z3_ast phi_a3 = Z3_mk_and(ctx, k, each_translator);
    return phi_a3;
}

//***** Phi_a
Z3_ast compute_phi_a(Z3_context ctx, Z3_ast Lit_x_ui[][k]) {
    Z3_ast phi_a1 = compute_phi_a1(ctx, Lit_x_ui);
    Z3_ast phi_a2 = compute_phi_a2(ctx, Lit_x_ui);
    Z3_ast phi_a3 = compute_phi_a3(ctx, Lit_x_ui);
    
    Z3_ast conj_phi_a[3] = {phi_a1, phi_a2, phi_a3};
    Z3_ast phi_a = Z3_mk_and(ctx, 3, conj_phi_a);
    return phi_a;
}



/***************************/
/********** Phi_b **********/
/***************************/

//***** Phi_b
Z3_ast compute_phi_b(Z3_context ctx, int j1, Z3_ast Lit_p_j1j2[][maxJ]) {
    Z3_ast each_j2[maxJ-1];
    int shift_1 = 0;
    for(int j2=0; j2<maxJ; j2++){
        if(j2 == j1)
            shift_1 = 1;
        else {
            Z3_ast p_j1j2 = Lit_p_j1j2[j1][j2];

            Z3_ast each_j3[maxJ-2];
            int shift_2 = 0;
            for(int j3=0; j3<maxJ; j3++){
                if (j3 == j1 || j3 == j2)
                    shift_2++;
                else {
                    Z3_ast p_j1j3 = Lit_p_j1j2[j1][j3];
                    each_j3[j3 - shift_2] = Z3_mk_not(ctx, p_j1j3);
                }
            }
            Z3_ast conj_neg_j1j3 = Z3_mk_and(ctx, maxJ-2, each_j3);

            Z3_ast conj[2] ={p_j1j2, conj_neg_j1j3};
            each_j2[j2 - shift_1] = Z3_mk_and(ctx,2, conj);
        }
    }
    Z3_ast phi_b = Z3_mk_or(ctx, maxJ-1, each_j2);
    return phi_b;
}



/***************************/
/********** Phi_c **********/
/***************************/

//***** Phi_c
Z3_ast compute_phi_c(Z3_context ctx, int j, Z3_ast Lit_l_jh[][maxH]) {      
    Z3_ast each_h1[maxH];
    for(int h1=0; h1<maxH; h1++){
        Z3_ast l_jh1 = Lit_l_jh[j][h1];

        Z3_ast each_h2[maxH-1];
        int shift = 0;
        for(int h2=0; h2<maxH; h2++) {
            if (h2 == h1)
                shift = 1;
            else {
                Z3_ast l_jh2 = Lit_l_jh[j][h2];
                each_h2[h2 - shift] = Z3_mk_not(ctx, l_jh2);
            }
        }
        Z3_ast conj_not_l_jh2 = Z3_mk_and(ctx, maxH-1, each_h2);

        Z3_ast conj[2] = {l_jh1, conj_not_l_jh2};
        each_h1[h1] = Z3_mk_and(ctx, 2, conj);
    } 
    Z3_ast phi_c = Z3_mk_or(ctx, maxH, each_h1);
    return phi_c;
}



/***************************/
/********** Phi_d **********/
/***************************/

//***** Phi_convertisseur
Z3_ast compute_phi_convertisseur(Z3_context ctx, BiConGraph biGraph, int j1, int j2, Z3_ast Lit_x_ui[][k]) {
    int n = orderG(biGraph.graph);

    int counter = 0;
    for (int u=0; u<n; u++) {
        for (int v=0; v<n; v++) {
            if (isEdgeHeterogeneous(biGraph, u, v)) {
                if (biGraph.homogeneousComponents[j1][u] && biGraph.homogeneousComponents[j2][v]) {
                    counter++;
                }
            }
        }
    }
    
    int index = 0;
    Z3_ast each_uv[counter];
    for (int u=0; u<n; u++) {
        for (int v=0; v<n; v++) {
            if (isEdgeHeterogeneous(biGraph, u, v)) {
                if (biGraph.homogeneousComponents[j1][u] && biGraph.homogeneousComponents[j2][v]) {
                    Z3_ast each_translator[k];
                    for (int i=0; i<k; i++) {
                        Z3_ast x_ui = Lit_x_ui[u][i];
                        Z3_ast x_vi = Lit_x_ui[v][i];
                        Z3_ast conj[2] = {x_ui, x_vi};
                        each_translator[i] = Z3_mk_or(ctx, 2, conj);
                    }
                    each_uv[index] = Z3_mk_or(ctx, k, each_translator);
                    index++;
                }
            }
        }
    }
    Z3_ast phi_convertisseur = Z3_mk_or(ctx, counter, each_uv);

    return phi_convertisseur;
}

//***** Phi_contigu
Z3_ast compute_phi_contigu(Z3_context ctx, int j1, int j2, Z3_ast Lit_l_jh[][maxH]) {
    Z3_ast each_h1[maxH-1];
    for (int h1=1; h1<maxH; h1++) {
        int h2 = h1 - 1;
        Z3_ast l_j1h1 = Lit_l_jh[j1][h1];
        Z3_ast l_j2h2 = Lit_l_jh[j2][h2];
        Z3_ast conj[2] = {l_j1h1, l_j2h2};
        each_h1[h2] = Z3_mk_and(ctx, 2, conj);
    }
    Z3_ast phi_contigu = Z3_mk_or(ctx, maxH-1, each_h1);
    return phi_contigu;
}

//***** Phi_lien_et_contigu
Z3_ast compute_phi_lien_et_contigu(Z3_context ctx, int j1, int j2, Z3_ast Lit_p_j1j2[][maxJ], Z3_ast Lit_l_jh[][maxH]) {
    Z3_ast p_j1j2 = Lit_p_j1j2[j1][j2];
    Z3_ast phi_contigu = compute_phi_contigu(ctx, j1, j2, Lit_l_jh);

    Z3_ast conj_phi_lc[2] = {p_j1j2, phi_contigu};
    Z3_ast phi_lien_et_contigu = Z3_mk_and(ctx, 2, conj_phi_lc);
    return phi_lien_et_contigu;
}

//***** Phi_d
Z3_ast compute_phi_d(Z3_context ctx, BiConGraph biGraph, int j1, int j2, Z3_ast Lit_x_ui[][k], Z3_ast Lit_p_j1j2[][maxJ], Z3_ast Lit_l_jh[][maxH]) {
    Z3_ast phi_convertisseur = compute_phi_convertisseur(ctx, biGraph, j1, j2, Lit_x_ui);
    Z3_ast phi_lien_et_contigu = compute_phi_lien_et_contigu(ctx, j1, j2, Lit_p_j1j2, Lit_l_jh);

    Z3_ast conj_phi_d[2] = {phi_convertisseur, phi_lien_et_contigu};
    Z3_ast phi_d = Z3_mk_and(ctx, 2, conj_phi_d);
    return phi_d;
}





/********************************/
/********** Phi_racine **********/
/********************************/

//***** Phi_r1
Z3_ast compute_phi_r1(Z3_context ctx, int j1, Z3_ast Lit_p_j1j2[][maxJ]) {
    Z3_ast each_j2[maxJ-1];
    int shift_1 = 0;
    for(int j2=0; j2<maxJ; j2++){
        if(j1 == j2)
            shift_1 = 1;
        else {
            Z3_ast p_j1j2 = Lit_p_j1j2[j1][j2];

            Z3_ast each_j3[maxJ-1];
            int shift_2 = 0;
            for(int j3=0; j3<maxJ; j3++) {
                if (j3 == j2)
                    shift_2++;
                else
                    each_j3[j3 - shift_2] = Lit_p_j1j2[j2][j3];
            }             
            Z3_ast disj_j3 = Z3_mk_or(ctx, maxJ-1, each_j3);
                                                 
            Z3_ast conj[2] = {Z3_mk_not(ctx, p_j1j2), disj_j3}; 
            each_j2[j2 - shift_1] = Z3_mk_and(ctx, 2, conj);
        }
    }
    Z3_ast phi_r1 = Z3_mk_and(ctx, maxJ-1, each_j2);   
    return phi_r1;
}

//***** Phi_r2
Z3_ast compute_phi_r2(Z3_context ctx, int j1, Z3_ast Lit_l_jh[][maxH]) {
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

//***** Phi_racine
Z3_ast compute_phi_racine(Z3_context ctx, int j, Z3_ast Lit_p_j1j2[][maxJ], Z3_ast Lit_l_jh[][maxH]) {
    Z3_ast phi_r1 = compute_phi_r1(ctx, j, Lit_p_j1j2);
    Z3_ast phi_r2 = compute_phi_r2(ctx, j, Lit_l_jh);

    Z3_ast conj[2] = {phi_r1, phi_r2};
    Z3_ast phi_racine = Z3_mk_and(ctx, 2, conj);
    return phi_racine;
}



/***********************************************/
/********** Phi_composante_quelconque **********/
/***********************************************/

//***** Phi_composante_quelconque
Z3_ast compute_phi_composante_quelconque (Z3_context ctx, BiConGraph biGraph, int j1, Z3_ast Lit_x_ui[][k], Z3_ast Lit_p_j1j2[][maxJ], Z3_ast Lit_l_jh[][maxH]) {
    Z3_ast phi_b = compute_phi_b(ctx, j1, Lit_p_j1j2);
    Z3_ast phi_c = compute_phi_c(ctx, j1, Lit_l_jh);

    Z3_ast each_phi_d[maxJ-1];
    int shift = 0;
    for (int j2=0; j2<maxJ; j2++) {
        if (j2 == j1)
            shift = 1;
        else {
            Z3_ast phi_d = compute_phi_d(ctx, biGraph, j1, j2, Lit_x_ui, Lit_p_j1j2, Lit_l_jh);
            each_phi_d[j2 - shift] = phi_d;
        }
    }
    Z3_ast disj_phi_d = Z3_mk_or(ctx, maxJ-1, each_phi_d);

    Z3_ast conj_cq[3] = {phi_b, phi_c, disj_phi_d};
    Z3_ast phi_composante_quelconque = Z3_mk_and(ctx, 3, conj_cq);
    return phi_composante_quelconque;
}





Z3_ast BiConReduction(Z3_context ctx, BiConGraph biGraph, int size){
    
    //***** Setting usefull global values
    n = orderG(biGraph.graph);
    k = size;
    maxJ = biGraph.numComponents;
    maxH = maxJ;

    //***** Creating tabs with each literals once and for all
    Z3_ast Lit_x_ui[n][k];
    Z3_ast Lit_p_j1j2[maxJ][maxJ];
    Z3_ast Lit_l_jh[maxJ][maxH];

    for (int u=0; u<n; u++)
        for (int i=0; i<k; i++)
            Lit_x_ui[u][i] = getVariableIsIthTranslator(ctx, u, i);

    for (int j1=0; j1<maxJ; j1++)
        for (int j2=0; j2<maxJ; j2++)
            Lit_p_j1j2[j1][j2] = getVariableParent(ctx, j1, j2);

    for (int j=0; j<maxJ; j++)
        for (int h=0; h<maxH; h++)
            Lit_l_jh[j][h] = getVariableLevelInSpanningTree(ctx, h, j);
    
    //***** Computing Formula
    Z3_ast phi_a = compute_phi_a(ctx, Lit_x_ui);

    Z3_ast each_CC[maxJ];
    for (int j=0; j<maxJ; j++) {
        Z3_ast phi_racine = compute_phi_racine(ctx, j, Lit_p_j1j2, Lit_l_jh);
        Z3_ast phi_composante_quelconque = compute_phi_composante_quelconque(ctx, biGraph, j, Lit_x_ui, Lit_p_j1j2, Lit_l_jh);
        Z3_ast disj_r_cq[2] = {phi_racine, phi_composante_quelconque};
        each_CC[j] = Z3_mk_or(ctx, 2, disj_r_cq);
    }
    Z3_ast conj_CC = Z3_mk_and(ctx, maxJ, each_CC);

    Z3_ast conj_g_k[2] = {phi_a, conj_CC};
    Z3_ast phi_G_k = Z3_mk_and(ctx, 2, conj_g_k);
    return phi_G_k;
}





void getTranslatorSetFromModel(Z3_context ctx, Z3_model model, BiConGraph *graph, int size){
    for (int u=0; u<n; u++) {
        for (int i=0; i<size; i++) {
            Z3_ast x_ui = getVariableIsIthTranslator(ctx, u, i);
            if (valueOfVarInModel(ctx, model, x_ui))
                addTranslator(graph, u);
        }
    }
    return;
}

