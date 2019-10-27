#include <stdio.h>
 
#define PI 3.141593
 
extern int Sin(double,double,double*);
 
int main(){
 
    int numIters=0;
    double x, e, valSen;
 
    printf("Entre com o ângulo, em graus: ");
    scanf("%lf",&x);
 
    printf("Entre com a diferença máxima: ");
    scanf("%lf",&e);
 
    numIters = Sin(x, e, &valSen);
   
    printf("Valor do sin = %lf\n", valSen);
    printf("Num de Iterações: %d\n", numIters);
   
    return 0;
}