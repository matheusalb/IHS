/*
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
// #include <allegro.h>
// #include <allegro_primitives.h>
// #include <allegro_font.h>
// #include <allegro_ttf.h>
// #include <allegro_image.h>

#define HEXPORT      1
#define SWITCHES_LED 2
#define HEX_DISPLAY  3
#define LED_RED      4
#define LED_GREEN    5
#define KEY_BUTTON   6
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 480
#define DIFERENCA_ANGULO 0

int initialize_allegro();
int print_floor();
int busca_vaga();
void print_obstaculos();

//Structs
struct vet_obs {
   ALLEGRO_TIMER *timer;
   int distancia;
   int angulo_obs;
} vet_obs;

//Globals
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_COLOR verde_padrao; //WARNING, nao use antes de definir a cor
ALLEGRO_BITMAP* linha_a_girar = NULL;
ALLEGRO_BITMAP* obstaculo_a_printar = NULL;
int angulo_servo_anterior = 0; //TODO IMPORTANTE considerar angulagem que eh sentido horario, 0 eh na vertical
int angulo_servo = 0; //TODO IMPORTANTE subtrair 90 do angulo recebido
int distancia_ultrassom = 200;
struct vet_obs obstaculos[1257];
int ledRed = 0, ledGreen = 0, hexBus = 4294967295, hexDisplay = 4294967295;
int hex[7];


int pot (int num, int exp) {
    int i;
    int result = 1;
    if (exp == 0) {
        return 1;
    }
    for (i = 0; i < exp; i++) {
        result = result * num;
    }
    return result;
}

void ledGradiente (int intensidade) {
    int i, j;
    i = j = 0;
    if (intensidade <= 0) { // 0 leds
        i = j = 0;
    }
    else if (intensidade <= 4) { // 1 led vermelho
        i = 131072;
        j = 0;
    }
    else if (intensidade <= 8) { // 2 leds vermelhos
        i = 196608;
        j = 0;
    }
    else if (intensidade <= 12) { // 3 leds vermelhos
        i = 229376;
        j = 0;
    }
    else if (intensidade <= 16) { // 4 leds vermelhos
        i = 245760;
        j = 0;
    }
    else if (intensidade <= 20) { // 5 leds vermelhos
        i = 253952;
        j = 0;
    }
    else if (intensidade <= 24) { // 6 leds vermelhos
        i = 258048;
        j = 0;
    }
    else if (intensidade <= 28) { // 7 leds vermelhos
        i = 260096;
        j = 0;
    }
    else if (intensidade <= 32) { // 8 leds vermelhos
        i = 261120;
        j = 0;
    }
    else if (intensidade <= 36) { // 9 leds vermelhos
        i = 261632;
        j = 0;
    }
    else if (intensidade <= 40) { // 10 leds vermelhos
        i = 261888;
        j = 0;
    }
    else if (intensidade <= 44) { // 11 leds vermelhos
        i = 262016;
        j = 0;
    }
    else if (intensidade <= 48) { // 12 leds vermelhos
        i = 262080;
        j = 0;
    }
    else if (intensidade <= 52) { // 13 leds vermelhos
        i = 262112;
        j = 0;
    }
    else if (intensidade <= 56) { // 14 leds vermelhos
        i = 262128;
        j = 0;
    }
    else if (intensidade <= 60) { // 15 leds vermelhos
        i = 262136;
        j = 0;
    }
    else if (intensidade <= 64) { // 16 leds vermelhos
        i = 262140;
        j = 0;
    }
    else if (intensidade <= 68) { // 17 leds vermelhos
        i = 262142;
        j = 0;
    }
    else if (intensidade <= 72) { // 18 leds vermelhos
        i = 262143;
        j = 0;
    }
    else if (intensidade <= 76) { // 1 leds verde
        i = 262143;
        j = 128;
    }
    else if (intensidade <= 80) { // 2 leds verde
        i = 262143;
        j = 192;
    }
    else if (intensidade <= 84) { // 3 leds verde
        i = 262143;
        j = 224;
    }
    else if (intensidade <= 88) { // 4 leds verde
        i = 262143;
        j = 240;
    }
    else if (intensidade <= 92) { // 5 leds verde
        i = 262143;
        j = 248;
    }
    else if (intensidade <= 94) { // 6 leds verde
        i = 262143;
        j = 252;
    }
    else if (intensidade <= 98) { // 7 leds verde
        i = 262143;
        j = 254;
    }
    else if (intensidade <= 100) { // 8 leds verde
        i = 262143;
        j = 255;
    }

    ledRed = i;
    ledGreen = j;
}

int *printBCD (int num) {
    switch (num) {
        case 0:
            hex[6] = 1;
            hex[5] = 0;
            hex[4] = 0;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 0;
            break;
        case 1:
            hex[6] = 1;
            hex[5] = 1;
            hex[4] = 1;
            hex[3] = 1;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 1;
            break;
        case 2:
            hex[6] = 0;
            hex[5] = 1;
            hex[4] = 0;
            hex[3] = 0;
            hex[2] = 1;
            hex[1] = 0;
            hex[0] = 0;
            break;
        case 3:
            hex[6] = 0;
            hex[5] = 1;
            hex[4] = 1;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 0;
            break;
        case 4:
            hex[6] = 0;
            hex[5] = 0;
            hex[4] = 1;
            hex[3] = 1;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 1;
            break;
        case 5:
            hex[6] = 0;
            hex[5] = 0;
            hex[4] = 1;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 1;
            hex[0] = 0;
            break;
        case 6:
            hex[6] = 0;
            hex[5] = 0;
            hex[4] = 0;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 1;
            hex[0] = 0;
            break;
        case 7:
            hex[6] = 1;
            hex[5] = 1;
            hex[4] = 1;
            hex[3] = 1;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 0;
            break;
        case 8:
            hex[6] = 0;
            hex[5] = 0;
            hex[4] = 0;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 0;
            break;
        case 9:
            hex[6] = 0;
            hex[5] = 0;
            hex[4] = 1;
            hex[3] = 0;
            hex[2] = 0;
            hex[1] = 0;
            hex[0] = 0;
            break;
        default:
            printf("Veio um meme no BCD ai...\n");
    }
}

void printHexport (int num) {
    int vetor[32];
    int resultado = 0;
    int modulo = 0, i = 0, j = 0, casa = 0;

    for (i = 0; i < 32; i++) {
        vetor[i] = 1;
    }

    while (num >= 10) {
        modulo = num % 10;
        printf("numero: [%d]\n", modulo);
        num = num/10;
        printBCD(modulo);
        for (i = 0; i < 7; i++, j++) {
            vetor[j] = hex[i];
        }
        j += 1;
        printf("Valor j[%d]\n", j);
    }

    modulo = num % 10;
    printf("numero: [%d]\n", modulo);
    num = num/10;
    printBCD(modulo);
    for (i = 0; i < 7; i++, j++) {
        vetor[j] = hex[i];
    }
    j += 1;
    printf("Valor j[%d]\n", j);

    for (i = 0; i < 32; i++) {
        if (vetor[i] == 1) {
            resultado += pot(2,i);
        }
    }

    printf("Numero final BCD: %d\n", resultado);
    hexBus = resultado;
}

int apagarLeds () {
    int i, apagado[32], hexDisplay = 0;
    for (i = 0; i < 32; i++) {
        apagado[i] = 1;
    }

    for (i = 0; i < 32; i++) {
        if (apagado[i] == 1) {
            hexDisplay += pot(2,i);
        }
    }
}

int main_dev() {
    int dev = open("/dev/de2i150_altera", O_RDWR);

    int intensidade, k, j, num;

    while (1) {
        // printf("\nDigite a intensidade: ");
        // scanf("%d", &intensidade);
        // ledGradiente(intensidade);
        // write(dev, &ledRed, LED_RED);
        // write(dev, &ledGreen, LED_GREEN);
        // printf("\nDigite o numero do BCD: ");
        // scanf("%d", &num);
        // printHexport(num);
        // write(dev, &hexBus, HEXPORT);
        // apagarLeds();
        // write(dev, &hexDisplay, HEX_DISPLAY);



    }

    close(dev);
    return 0;
}

//TODO NAO IMPORTANTE trocar numeros por defines
int main(){
  printf("%0x\n%0x\n", ALLEGRO_VERSION_INT, al_get_allegro_version());
   if(initialize_allegro() == -1) return -1;
   int dev = open("/dev/de2i150_altera", O_RDWR);
   int variacao = 1;
   int permanece = 1; 
   int vaga = 0;

   apagarLeds();
   // write(dev, &hexDisplay, HEX_DISPLAY);

   while(permanece){
      print_floor();
      
      if(angulo_servo != angulo_servo_anterior){ // uma nova parte será renderizada
         al_draw_rotated_bitmap(linha_a_girar,4,440,450,480,angulo_servo*ALLEGRO_PI/180,0); // barrra de varredura
         angulo_servo_anterior = angulo_servo;         
      }
      angulo_servo += variacao; //TODO IMPORTANTE receber o angulo da placa
      if(angulo_servo == 90 || angulo_servo == -90) variacao = -1*variacao;

      if(distancia_ultrassom){
         vaga = busca_vaga();
         al_start_timer(obstaculos[vaga].timer);
         obstaculos[vaga].distancia = distancia_ultrassom;
         obstaculos[vaga].angulo_obs = angulo_servo;
         distancia_ultrassom = 0; //TODO IMPORTANTE definir se a gt vai setar 0 caso nao tenha obstaculo, ou algo do tipo
      }

      int intensidade, k, j, num;

       read(dev, &distancia_ultrassom, SWITCHES_LED);
      // ledGradiente(distancia_ultrassom);
       write(dev, &ledRed, LED_RED);
      // write(dev, &ledGreen, LED_GREEN);
      // printHexport(num);
      // write(dev, &hexBus, HEXPORT);


      print_obstaculos();

      //FUNCOES DE TIMER
      // al_get_timer_count(ALLEGRO_TIMER *timer);
      // al_stop_timer(ALLEGRO_TIMER *timer);
      // al_start_timer(ALLEGRO_TIMER *timer);
      // bool al_get_timer_started(ALLEGRO_TIMER *timer);

      al_flip_display(); //joga o display depois da ultima chamada dessa funcao
      al_rest(0.000000000000000000000000000000000001);
   }
   //TODO NAO IMPORTANTE cortar bitmap para formato menor?
 
   al_flip_display(); //joga o display depois da ultima chamada dessa funcao

   al_rest(20);
     
   al_destroy_display(display); //TODO NAO IMPORTANTE separar numa funcao so
   al_destroy_font(font);
   al_destroy_bitmap(linha_a_girar);
   for(int i = 0; i < 1257 ; i++) {
      al_destroy_timer(obstaculos[i].timer);
   }

  close(dev);
  return 0;
}
*/

// #include <unistd.h>
// #include <stdio.h>
// #include <fcntl.h>

// #define HEX1234       1
// #define HEX5678       2
// #define REDLEDS       3
// #define GREENLEDS     4
// #define SWITCHES      5
// #define BUTTONS       6

// unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
//                             0x66, 0x6D, 0x7D, 0x07, 
//                             0x7F, 0x6F, 0x77, 0x7C,
//                             0x39, 0x5E, 0x79, 0x71};

// int main() {
//   int i, j, k;

//   int dev = open("/dev/de2i150_altera", O_RDWR);

//   unsigned value;
//   write(dev, &hexdigit[2], HEX1234);
//   while (1)
//   {
//     read(dev, &value, BUTTONS);
//     printf("%u\n", ((~value) << 28) >> 28);
//   }

//   close(dev);
//   return 0;
// }

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>

enum OPS {HEX_RIGHT,HEX_LEFT,RED_LEDS,GREEN_LEDS,BUTTONS,SWITCHES};
// #define HEX1234       0
// #define HEX5678       2
// #define REDLEDS       3
// #define GREENLEDS     4
// #define SWITCHES      5
// #define BUTTONS       6

uint32_t number = 0x40794079;
uint32_t zero = 0x40404040;
uint32_t zerol = 0;
uint32_t onel = 1;
uint32_t numberSwitch = 0;
uint32_t numberButton = 15;
unsigned char hexdigit[] = {0x24, 0x79, 0x40, 0x40,
                            0x40, 0x40, 0x40, 0x40, 
                            0xFF, 0x40, 0x40, 0x40,
                      0xFF, 0x5E, 0x79, 0x71};

void wait(void){
  int i;
  for(i=0;i>-1;i++){
  }
  
  return;
}

void real_write(int debug,int dev,unsigned char * buffer,int opt){
  int ret;
  wait();
  ret = write(dev,buffer,opt);
  ret = write(dev,buffer,opt);
  if( ret != -1) printf("Escrita ok!\n");
}

int main() {
  int i, j = 1, k;
  int ret = 0;

  int dev = open("/dev/de2i150_altera", O_RDWR);

  printf("%d\n",dev);

  real_write(0,dev,&zerol,RED_LEDS);
  real_write(0,dev,&zerol,GREEN_LEDS);
  real_write(0,dev,&number,HEX_RIGHT);

  while(1){
    ret = read(dev,&numberSwitch,SWITCHES);
    if(ret == -1){ 
      printf("deu erro na leitura\n");
      break;
    }
    if(numberSwitch == 2){ 
      real_write(0,dev,&onel,RED_LEDS);
      break;
    }

    ret = read(dev,&numberButton,BUTTONS);
    if(ret == -1){ 
      printf("deu erro na leitura\n");
      break;
    }
    if(numberButton == 11){ 
      real_write(0,dev,&onel,RED_LEDS);
      break;
    }
    printf("%d %d\n",numberSwitch, numberButton);
  }

  
  
  real_write(0,dev,&zero,HEX_RIGHT);

  close(dev);
  return 0;
}



