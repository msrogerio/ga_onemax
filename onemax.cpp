#include <iostream>
#include <vector>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <string.h>

/***
    AUTHOR: Marlon da Silva Rogério
    DATA: 25 ABR 2021
    DISCIPLINA: Tópicos Especiais em Inteligência Computacional 
    DESCRICAO: Agoritmos Genéticos | Onemax | Mimização
***/

using namespace std;

long geracoes = 100; //numero de geracoes
const int tam_pop = 100; //tamanho da populacao
const int dimensoes_var=100; //numero de variaveis de decisao
const int dimensoes_obj=1; //numero de objetivos
const double prob_mutacao=0.3; //probabilidade de mutacao
struct Individuo {
    int x[dimensoes_var]; //vetor de variaveis de decisao
    double fx[dimensoes_obj]; //vetor de objetivos
};

Individuo populacao_P[tam_pop]; //Populacao pai P
Individuo populacao_Q[tam_pop]; //Populacao filha Q
Individuo melhor; //vai conter a melhor solucao obtida ate agora


//prototipacao das funcoes
void inicializacao(); //inicializa todas as solucoes aleatoriamente
void aptidao(Individuo *ind); //calcula o fitness ou valor objetivo
void selecao(Individuo *ind, Individuo *populacao); //seleciona um individuo para cruzamento
void cruzamento(Individuo *filho1, Individuo *filho2, Individuo *pai1, Individuo *pai2); //gera dois filhos a partir de dois pais
void mutacao(Individuo *ind); //faz uma alteracao aleatoria no individuo

int main(const int argc, const char* argv[]){
    srand (time(NULL)); //inicializa a semente aleatória com o tempo atual

    inicializacao();
    for (int i=0; i<tam_pop; i++){
        cout << " [ POP P ] - IND 0" << i << " >>> ";
        for (int j=0; j<dimensoes_var; j++)
            cout << populacao_P[i].x[j];
        cout << endl;
    }
    populacao_P[0].fx[0]=DBL_MAX;
    memcpy(&melhor, &populacao_P[0], sizeof(Individuo));//inicializa a melhor solucao com uma solucao qualquer
    
    for(long g=0;g<geracoes;g++){//laco principal
        for(int i=0;i<tam_pop;i++){ //laco para o calculo do fitness e atualizacao da melhor solucao
            aptidao(&populacao_P[i]);
            if(populacao_P[i].fx[0] < melhor.fx[0])
                memcpy(&melhor, &populacao_P[i], sizeof(Individuo));
        }
        for(int i=0;i<tam_pop/2;i++){ //laco para a geracao da nova populacao
            Individuo pai1, pai2, filho1, filho2;
            selecao(&pai1, populacao_P);
            selecao(&pai2, populacao_P);
            cruzamento(&filho1, &filho2, &pai1, &pai2);
            mutacao(&filho1);
            mutacao(&filho2);
            memcpy(&populacao_Q[i], &filho1, sizeof(Individuo));
            memcpy(&populacao_Q[(tam_pop/2)+i], &filho2, sizeof(Individuo));
        }
        memcpy(&populacao_P, &populacao_Q, sizeof(Individuo)*tam_pop);
        cout << endl;
        cout << "[ 1.1 ] - GERACAO -> " << g;
        cout << endl;
        cout << "[ 1.2 ] - MELHOR DA GERACAO =>> " << melhor.fx[0];
        cout << endl;
        for (int i=0; i<dimensoes_var; i++){
            cout << melhor.x[i];
        }
        cout << endl;
        if (melhor.fx[0] == 0)
            break;
    }
    cout << endl;
    cout << "[ END ] - SOLUCAO FINAL " << melhor.fx[0];
    cout << endl;
    for(int j=0;j<dimensoes_var;j++){
        cout << melhor.x[j];
    }
    cout << endl;
}


void inicializacao(){
    for(int i=0; i < tam_pop; i++ ){
        Individuo ind;
        ind.fx[0]=0;
        for (int j=0; j < dimensoes_var; j++){
            int num = rand() % 2;
            if (num==0)
                ind.fx[0]=1;
            ind.x[j] = num;
        }
        populacao_P[i] = ind;
    }
}


void aptidao(Individuo *ind){
    ind->fx[0]=0;
    for(int j=0;j<dimensoes_var;j++){
        if(ind->x[j]==0)
            ind->fx[0]+=1;
    }
}


void selecao(Individuo *ind, Individuo *populacao){ //torneio binario
    int tam_torneio = 7;
    int indice_ind1 = rand()%tam_pop;
    int indice_ind2;
    Individuo ind_1 = populacao[indice_ind1];
    Individuo ind_2;
    do {
        indice_ind2 = rand()%tam_pop;
    }while (indice_ind1 == indice_ind2);
    ind_2 = populacao[indice_ind2];
    for(int i = 0; i < tam_torneio; i++){
        if (ind_1.fx[0] > ind_2.fx[0]){
            memcpy(ind, &ind_2, sizeof(Individuo));
        }else{
            memcpy(ind, &ind_1, sizeof(Individuo));
        }
    } 
}


void cruzamento(Individuo *filho1, Individuo *filho2, Individuo *pai1, Individuo *pai2){ //cruzamento uniforme
    double valor_aleatorio = ((double) rand()/((double)RAND_MAX));
    double prob_cruzamento = 0.5;
    for (int i=0; i < dimensoes_var; i++){
        if (prob_cruzamento > valor_aleatorio ){
            filho1->x[i] = pai1->x[i];
            filho2->x[i] = pai2->x[i];
        }
    }
    for (int i=0; i < dimensoes_var; i++){
        if (prob_cruzamento > valor_aleatorio ){
            filho1->x[i] = pai2->x[i];
            filho2->x[i] = pai1->x[i];
        }
    }
}


void mutacao(Individuo *ind){ 
    int x = rand()%dimensoes_var;
    double valor_aleatorio = ((double) rand()/(double)RAND_MAX);
    if (valor_aleatorio < prob_mutacao){     
        if (ind->x[x] == 0){
            ind->x[x]=1;
        }
    }
}