#include <stdio.h>

#define PI 3.141593

// extern float sin(int,int);

float convertAng (int x){
    return (PI*x)/180;    
}

int main (){
    int x, e, numIter=0;
    float angRad, val = 0;

    printf("Digite o ângulo, em graus, e a diferença máxima:");
    scanf("%d %d", &x, &e);

    angRad = convertAng(x);

    // val = sin(angRad,e,&numIter);

    printf("Val = %f, NumIter = %d",val,numIter);
    return 0;

}

