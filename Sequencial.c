#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

double *x, *y; //Variaveis para armazenar os vetores

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

    if(rx == 0 || ry == 0){ //Protecao para nao haver divisao por 0
        return -2;
    }

    cc = ((n * xy) - (x * y)) / (sqrt(rx) * sqrt(ry));

    return cc;
}

int main(int argc, char * argv[]){

    clock_t timeI = clock();	// Definindo o tempo de execução do cálculo do coeficiente

    FILE * descritorArquivo; 	//descritor do arquivo de entrada
    size_t ret; 				//retorno da funcao de leitura no arquivo de entrada


    //Verifica se as entradas estao corretas
    if(argc != 3){
        printf("Digite: %s <TamDoVetor> <NomeDoArquivo>", argv[0]);
        return 1;
    }

    int tam = atoi(argv[1]);

    double sx, sy, qx, qy, xy, cc; //Variaveis para armazenar os somatorios

    x = (double *) malloc(tam*sizeof(double));
    if(x == NULL){
        printf("Erro ao alocar memoria");
        return 2;
    }
    y = (double *) malloc(tam*sizeof(double));
    if(y == NULL){
        printf("Erro ao alocar memoria");
        return 2;
    }

    // abre o arquivo para leitura binaria
    descritorArquivo = fopen(argv[2], "rb");
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


    //Realiza as funcoes necessarias para o calculo
    sx = soma(x, tam);
    sy = soma(y, tam);
    qx = quad(x, tam);
    qy = quad(y, tam);
    xy = mult(x, y, tam);
    cc = coef(sx, sy, xy, qx, qy, tam);

    //Verificacao caso haja divisao por 0
    if(cc == -2){
        printf("Valor indeterminado\n");
        return -1;
    }

    //Finaliza a marcacao de tempo
    clock_t timeF = clock();
    double time = ((double) (timeF - timeI)) / CLOCKS_PER_SEC;

    printf("\nCoeficiente de correlacao: %lf\nTempo de execucao: %lf\n", cc, time);

    //libera a memoria alocada
    free(x);
    free(y);
   	fclose(descritorArquivo);
    return 0;
}
