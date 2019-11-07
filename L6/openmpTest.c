#include<stdio.h>
#include<stdlib.h>
int checaCirculo(double x,double y){ //Função que checa se tá no círculo

    if(x*x+y*y <= 1){
        return 1; // Se for menor que 1, então retorna 1
    }
    else 
        return 0; // Retorna 0
    
}


int main(){

    int N;
    printf("digite o N necessario\n"); //Pega o N
    scanf("%d",&N);

    
    double HFinal=0;
    double soma = 0;// Declara a soma e a integral de H final


    #pragma omp parallel num_threads(N) reduction(+:soma)
    {
        
        printf("Hello %d \n",omp_get_thread_num());
        double XAl = (double)rand() / (double)RAND_MAX ; 
        double YAL = (double)rand() / (double)RAND_MAX ; //Pegando um ponto aleatório
        soma += checaCirculo(XAl,YAL); //Função que pega o H
    }

    
    HFinal = (4*soma)/N; // Multiplica e divide por 
    printf("%f e N = %d e soma = %f\n",HFinal,N,soma);
}
