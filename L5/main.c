#include <stdio.h>

#define PI 3.141593

// extern float sin(int,int);

float convertAng (int x){
    return (PI*x)/180;    
}

extern void Sin(double,double,double*);
int main (){
    int  numIter=0;
    double x,e,val=0;

    printf("Digite o ângulo, em graus, e a diferença máxima:");
    scanf("%lf %lf", &x, &e);



    Sin(x,e,&val);

    printf("Val = %f, NumIter = %d",val,numIter);
    return 0;

}

