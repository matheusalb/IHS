#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>

enum OPS {HEX_RIGHT,HEX_LEFT,RED_LEDS,GREEN_LEDS,BUTTONS,SWITCHES};

uint32_t number = 0x40794079;
uint32_t zero = 0x40404040;
uint32_t zerol = 0;
uint32_t onel = 1;
uint32_t numberSwitch = 0;
uint32_t numberButton = 20;
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
    // ret = read(dev,&numberSwitch,BUTTONS);
    // if(ret == -1){ 
    //   printf("deu erro na leitura\n");
    //   break;
    // }
    
    // printf("%d\n",numberSwitch);
    // if(numberSwitch == 2){ 
    //   real_write(0,dev,&onel,RED_LEDS);
    //   break;
    // }

    ret = read(dev,&numberButton, SWITCHES);
    if(ret == -1){ 
      printf("deu erro na leitura\n");
      break;
    }

    printf("%d\n",numberButton);
    if(numberButton == 2){ 
      real_write(0,dev,&onel,RED_LEDS);
      break;
   }

  }

  
  
  real_write(0,dev,&zero,HEX_RIGHT);

  close(dev);
  return 0;
}



