#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


double *x, *y; //Variaveis para armazenar os arrays
double sx, sy; //Somatorio de x e y
double qx, qy; //Somatorio quad x e y
double xy; //Somatorio do produto de x e y
int tam; //Tamanho dos arrays
int nthreads; //Num de threads



//Recebe um vetor e seu tamanho
//Retorna a soma de seus valores
double soma(double var[], int n){
    double soma = 0;
    for(int i = 0; i < n; i++){
        soma += var[i];
    }
    return soma;
}

//Recebe um vetor e seu tamanho
//Retorna o somatorio do quadrado de seus valores
double quad(double var[], int n){
    double quad = 0;
    for(int i = 0; i < n; i++){
        quad = quad + (var[i] * var[i]);
    }
    return quad;
}

//Recebe dois vetores e seus tamanhos(tamanhos iguais)
//Rretorna o somatorio da multiplicacao de seus valores
double mult(double var1[], double var2[], int n){
    double multi = 0;
    for(int i = 0; i < n; i++){
        multi = multi + (var1[i] * var2[i]);
    }
    return multi;
}

//Recebe os somatorios dos quadrados de x e y e da multiplicacao x * y e o tamanho dos vetores
//Retorna o coeficiente de correlacao das duas variaveis
double coef(double x, double y, double xy, double xq, double yq, int n){
    double cc;
    double rx, ry; //Sera calculada a raiz desses valores na eq final

    rx = (n * xq) - (x * x);
    ry = (n * yq) - (y * y);

    if(rx == 0 || ry == 0){
        return -2;
    }

    cc = ((n * xy) - (x * y)) / (sqrt(rx) * sqrt(ry));

    return cc;
}

void *func(void *arg){
    int id = (int) arg;

    if(id == 0){
        sx = soma(x, tam);
        id += nthreads;
    }

    if(id == 1){
        sy = soma(y, tam);
        id += nthreads;
    }

    if(id == 2){
        xy = mult(x, y, tam);
        id += nthreads;
    }

    if(id == 3){
        qx = quad(x, tam);
        id += nthreads;
    }

    if(id == 4){
        qy = quad(y, tam);
        id += nthreads;
    }

    return NULL;
}





int main(int argc, char * argv[]){

    clock_t timeI, timeF;// Definindo o tempo de execução do cálculo do coeficiente
    timeI = clock();
    double timeT;
    FILE * descritorArquivo; 	//descritor do arquivo de entrada
    size_t ret; 				//retorno da funcao de leitura no arquivo de entrada


    //Verifica se as entradas estao corretas
    if(argc != 4){
        printf("Digite: %s <NumDeThreads> <TamDoVetor> <ArquivoDeEntrada>", argv[0]);
        return 1;
    }


    nthreads = atoi(argv[1]);//armazena o num de threads
    tam = atoi(argv[2]);//armazena o tamanho do vet


    pthread_t tid_sistema[nthreads];//Aloca memoria para criacao das threads

    //Aloca memoria para os vetores
    x = (double *) malloc(tam*sizeof(double));
    if (x == NULL)
    {
        printf("Erro ao alocar memoria para x!\n");
        return 2;
    }
    y = (double *) malloc(tam*sizeof(double));
    if (y == NULL)
    {
        printf("Erro ao alocar memoria para y!\n");
        return 2;
    }

    // abre o arquivo para leitura binaria
    descritorArquivo = fopen(argv[3], "rb");
    if(!descritorArquivo) {
       fprintf(stderr, "Erro de abertura do arquivo\n");
       return 3;
    }

    // le os valores de x
    ret = fread(x, sizeof(double), tam, descritorArquivo);
    if(!ret) {
       fprintf(stderr, "Erro de leitura dos valores de x \n");
       return 4;
    }

    // le os valores de y
    ret = fread(y, sizeof(double), tam, descritorArquivo);
    if(!ret) {
       fprintf(stderr, "Erro de leitura dos valores de y \n");
       return 4;
    }


    //Cria as threads
    for(int i = 0; i < nthreads; i++){
        if(pthread_create(&tid_sistema[i], NULL, func, (void*) i)){
            fprintf(stderr, "Falha ao criar thread\n");
            return 3;
        }
    }

    //Verifica se todas as threads ja terminaram
    for (int j=0; j<nthreads; j++) {
        if (pthread_join(tid_sistema[j], NULL)) {
            printf("Erro pthread_join()\n");
            return 4;
        }
    }

    //main faz o calculo final
    double cc = coef(sx, sy, xy, qx, qy, tam);

    //finaliza a marcacao de tempo
    timeF = clock();
    timeT = ((double) (timeF - timeI)) / CLOCKS_PER_SEC;
    printf("Coeficiente de correlacao: %lf\nTempo de execucao: %lf", cc, timeT);

    //libera memoria alocada
    free(x);
    free(y);
    fclose(descritorArquivo);

    return 0;

}
