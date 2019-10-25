#include <stdio.h>

#define PI 3.141593

// extern float sin(int,int);



extern void Sin(double,double,double*);
int main (){
    int  numIter=0;
    double x=270.0,e=0.0000000001,val=0;

    printf("Digite o ângulo, em graus, e a diferença máxima:");
    scanf("%lf %lf", &x, &e);

    


    



    Sin(x,e,&val);

    printf("Valor do sin = %lf \n",val);


    return 0;

}

