#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
//#include <random>

#include <cmath>
//#include "de150interface.h"
using namespace std;
//using namespace de150interface;

const int tab_size = 3;
// random_device rand_device;
// mt19937 rand_engine(rand_device());

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

void escrever7Seg(unsigned char numero, bool lado = 1) {
  // lado == 1, hexa do lado esquerdo, lado == 0 direto;

  int dev = open("/dev/de2i150_altera", O_RDWR);
  write(dev, &numero, HEX_RIGHT);

  close(dev);
  return;
}

uint32_t lerSwitchComButton() {
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
  close(dev);
  return op_num;
}

// Retorna o id do butao (3, 2, 1, 0) na placa;
int lerButton() {
  int i, j = 1, k;
  int ret = 0;
  uint32_t op_num, ant;

  int dev = open("/dev/de2i150_altera", O_RDWR);
  bool count = true;


  while(count){
    ant = 15;
    numberButton = ant;

    while(numberButton == ant) read(dev, &numberButton, BUTTONS);

    if(numberButton >= 0 && numberButton < 15){
      printf("leu: %d\n", numberButton);
      count = false;
    }
  }

  close(dev);

  if(numberButton == 7) return 3;
  else if(numberButton == 11) return 2;
  else if(numberButton == 13) return 1;
  else if(numberButton == 14) return 0;
  else return -1;
}



void wait(void){
  int i;
  for(i=0;i>-1;i++){
  }
  
  return;
}

void real_write(int debug,int dev,unsigned char* buffer,int opt){
  int ret;
  wait();
  ret = write(dev,buffer,opt);
  ret = write(dev,buffer,opt);
  if( ret != -1) printf("Escrita ok!\n");
}


string random_element(vector<string> &choices){
  // uniform_int_distribution<int> dist(0, choices.size()-1);
  // return choices[dist(rand_engine)];
  return choices[0];
}

map<string, vector<string> > learned;

void clean(){
  for(int i=0; i<80; i++){
    printf("\n");
  }
}

void print_field(string tab){
  for(int i=0; i<tab_size*tab_size; i++){
    if(i%tab_size == 0) printf("\n\t");
    if(tab[i] == '0') printf("_");
    else if(tab[i] == '1') printf("X");
    else printf("O");
  }
  printf("\n");
}

vector<string> learn_state(string state, int turn){
  vector<string> answer;
  string to;

  if(turn == 0){
    for(int i=tab_size; i<tab_size*tab_size; i++){
      if(state[i] == '1' && state[i-tab_size] == '0'){
        to = state;
        swap(to[i-tab_size], to[i]);
        answer.push_back(to);
      }
      if(i%tab_size >= 1 && state[i] == '1' && state[i-tab_size-1] == '2'){
        to = state;
        to[i] = '0';
        to[i-tab_size-1] = '1';
        answer.push_back(to);
      }
      if(i%tab_size <= 1 && state[i] == '1' && state[i-tab_size+1] == '2'){
        to = state;
        to[i] = '0';
        to[i-tab_size+1] = '1';
        answer.push_back(to);
      }
    }
  }
  else{
    for(int i=0; i<tab_size*(tab_size-1); i++){
      if(state[i] == '2' && state[i+tab_size] == '0'){
        to = state;
        swap(to[i], to[i+tab_size]);
        answer.push_back(to);
      }
      if(i%tab_size >= 1 && state[i] == '2' && state[i+tab_size-1] == '1'){
        to = state;
        to[i] = '0';
        to[i+tab_size-1] = '2';
        answer.push_back(to);
      }
      if(i%tab_size <= tab_size-2 && state[i] == '2' && state[i+tab_size+1] == '1'){
        to = state;
        to[i] = '0';
        to[i+tab_size+1] = '2';
        answer.push_back(to);
      }
    }
  }

  return answer;
}

vector<string> remove_element(string target, vector<string> from){
  for(int i=0; i<from.size(); i++){
    if(from[i] == target){
      swap(from[i], from.back());
      break;
    }
  }
  from.pop_back();
  return from;
}

bool is_final_state(string state){
  for(int i=0; i<tab_size; i++) if(state[i] == '1') return 1;
  for(int i=tab_size*(tab_size-1); i<tab_size*tab_size; i++) if(state[i] == '2') return 1;
  return 0;
}

string initial_state(){
  string answer = "";
  for(int i=0; i<tab_size; i++) answer += "2";
  for(int i=0; i<tab_size*(tab_size-2); i++) answer += "0";
  for(int i=0; i<tab_size; i++) answer += "1";
  return answer;
}

bool play(string state = initial_state(), int turn = 0){

  clean();
  printf("\nCurrent Field:\n");
  print_field(state);

  vector<string> possibleMoves = learn_state(state, turn);
  if(is_final_state(state) || possibleMoves.size() == 0){
    if(turn == 0){
      printf("\nYou lost :(\n");
      return 0;
    }
    else {
      printf("\nYou Won :)\n");
      return 1;
    }
  }

  if(turn == 0){
    printf("\nPossible Choices:\n");
    for(int i=0; i<possibleMoves.size(); i++){
      printf("\n%d:",i+1);
      print_field(possibleMoves[i]);
    }

    int choice = 0;
    int nPossibleMoves = (int) pow(2, possibleMoves.size()-1);
    while(choice <= 0 || choice > nPossibleMoves){
      printf("\n>> ");
      choice = (int) lerSwitchComButton();  //scanf("%d",&choice);
    }
    printf("sai do while, choice = %d && nPossibleMoves = %d \n", choice, nPossibleMoves);


    string chosen = possibleMoves[log2(choice)];
    return play(chosen, 1);
  }
  else{
    if(!learned.count(state)) learned[state] = possibleMoves;
    else possibleMoves = learned[state];

    printf("\nMachine Deciding from...\n");
    for(int i=0; i<possibleMoves.size(); i++){
      print_field(possibleMoves[i]);
    }
    printf("\n\nPress '1' to continue\n>> ");
    int opt;
    opt = (int) lerSwitchComButton();//scanf("%d",&opt);

    string chosen = random_element(possibleMoves);
    bool erase = play(chosen, 0);

    if(erase == 1 && possibleMoves.size() > 1){
      printf("\nComputer learned not to go to\n");
      print_field(chosen);
      printf("\nafter\n");
      print_field(state);
      learned[state] = remove_element(chosen, possibleMoves);
      erase = 0;
    }

    return erase;
  }
}

int main(){
  //  clean();
  // int opt;
  // printf("\nPress '1' to play\n>> ");
  // opt = (int) lerSwitchComButton();//scanf("%d",&opt);
  // while(opt == 1){
  //   play();
  //   printf("\nPress '1' to play again\n>> ");
  //   opt = (int) lerSwitchComButton();
  // }

  int dev = open("/dev/de2i150_altera", O_RDWR);
  real_write(0,dev,&hexdigit[8],HEX_RIGHT);
  close(dev);
   


  return 0;
}
