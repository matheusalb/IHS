#include "de150interface.h"

uint32_t lerSwitch() {
  int i, j = 1, k;
  int ret = 0;
  uint32_t op_num, ant;

  int dev = open("/dev/de2i150_altera", O_RDWR);

  //printf("%d\n",dev);

  // real_write(0,dev,&zerol,RED_LEDS);
  // real_write(0,dev,&zerol,GREEN_LEDS);
  // real_write(0,dev,&number,HEX_RIGHT);
  bool cont = true;

  while(cont){

    read(dev,&ant, BUTTONS);
    numberButton = ant;
    while(numberButton == ant) read(dev,&numberButton, BUTTONS);

    if(numberButton == 7){
      read(dev,&op_num, SWITCHES);
      printf("Switches em : %d\n", op_num);
      cont = false;
    }
  }
  
  //real_write(0,dev,&zero,HEX_RIGHT);

  close(dev);
  return op_num;
}