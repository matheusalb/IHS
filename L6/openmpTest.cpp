#include <bits/stdc++.h>
#include <omp.h>

int getRandom() {
    // Pega um ponto de retangulo de area 2 que envolve o círculo

    double x = (double)rand()/(double)RAND_MAX;
    double y = (double)rand()/(double)RAND_MAX; // Pega um ponto alestório

    double val = pow(x,2.) + pow(y,2.); // Pega a distancia a origem

    if( val <= 1.) //Verifica se esta é menor ou igual a 1
        return 1;
    else
        return 0;
}


int main() {

    int N = 9000;
    double somatorio = 0;


    omp_lock_t lock;  
    omp_init_lock(&lock); //Inicializa lock
    
    #pragma omp parallel num_threads(N)  //Declara o numero de threads
    {
        #pragma omp for nowait  // Não espera
            for(int i = 1; i < N; i++) {
            
                omp_set_lock(&lock); //Faz um lock para impedir que threads acessem um mesmo recurso compartilhado
                
                somatorio += getRandom();
                omp_unset_lock(&lock);
            }
    }
    somatorio = 4.*somatorio/(double)N; // Executa a fórmula

    std::cout << somatorio << std::endl; // Printa

    return 0;
}