// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector> 
#include <sstream>
#include <stdint.h>
#include "../lib/de150.cpp"
#include <allegro5/allegro_font.h>
#include "app.cpp"
#include <allegro5/allegro_ttf.h>

// Atributos da tela
const int LARGURA_TELA = 400;
const int ALTURA_TELA = 250;
const float FPS = 10;

using namespace std;

int main(void)
{
  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  ALLEGRO_BITMAP *snake = NULL, *hexspawn = 0;
  ALLEGRO_FONT *fonte = NULL;
  // Flag que condicionará nosso looping
  int sair = 0;
 
  if (!al_init())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }

  // Inicialização do add-on para uso de fontes
  al_init_font_addon();

  // Inicialização do add-on para uso de fontes True Type
  if (!al_init_ttf_addon())
  {
    fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
    return -1;
  }
 
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela)
  {
    fprintf(stderr, "Falha ao criar janela.\n");
    return -1;
  }
 
  // Configura o título da janela
  al_set_window_title(janela, "Menu - Projeto/IHS - 2019.2");
 
  // Torna apto o uso de mouse na aplicação
  if (!al_install_mouse())
  {
    fprintf(stderr, "Falha ao inicializar o mouse.\n");
    al_destroy_display(janela);
    return -1;
  }
 
  // Atribui o cursor padrão do sistema para ser usado
  if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
  {
    fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
    al_destroy_display(janela);
    return -1;
  }

  fonte = al_load_font("arial_narrow_7.ttf", 100, 0);
  if (!fonte)
  {
    al_destroy_display(janela);
    fprintf(stderr, "Falha ao carregar fonte.\n");
    return -1;
  }
  printf("fonte carregada\n");

  al_draw_text(fonte, al_map_rgb(255, 255, 255), 100, 125, 0, "Hexapawn");
 
  // Alocamos o retângulo central da tela
  hexspawn = al_create_bitmap(100,50);
  if (!hexspawn)
  {
    fprintf(stderr, "Falha ao criar bitmap.\n");
    al_destroy_display(janela);
    return -1;
  }
 
  // Alocamos o botão para fechar a aplicação
  snake = al_create_bitmap(100, 50);
  if (!snake)
  {
    fprintf(stderr, "Falha ao criar botão de saída.\n");
    al_destroy_bitmap(hexspawn);
    al_destroy_display(janela);
    return -1;
  }
 
  fila_eventos = al_create_event_queue();
  if (!fila_eventos)
  {
    fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
    al_destroy_display(janela);
    return -1;
  }
 
  // Dizemos que vamos tratar os eventos vindos do mouse
  al_register_event_source(fila_eventos, al_get_mouse_event_source());
 
  // Flag indicando se o mouse está sobre o retângulo central
  int in_hexspawn = 0;
  int in_snake = 0;
  int run_snake = 0;
  int run_hexspawn = 0;
  while (1)
  {
    // Verificamos se há eventos na fila
    while (!al_is_event_queue_empty(fila_eventos))
    {
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);
 
      // Se o evento foi de movimentação do mouse
      if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
      {
        // Verificamos se ele está sobre a região do retângulo central
        if (evento.mouse.x >= 50 && evento.mouse.x <= 150 &&evento.mouse.y >= 100 &&
            evento.mouse.y <= 150)
        {
          in_hexspawn = 1;
        }
        else
        {
          in_hexspawn = 0;
        }
        // Verificamos se ele está sobre a região do retângulo SNAKE
        if (evento.mouse.x >= 250 && evento.mouse.x <= 350 && evento.mouse.y >= 100 &&
            evento.mouse.y <= 150)
        {
          in_snake = 1;
        }
        else
        {
          in_snake = 0;
        }
      }
      // Ou se o evento foi um clique do mouse
      else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
      {
        if (evento.mouse.x >= 250 && evento.mouse.x <= 350 && evento.mouse.y >= 100 &&
            evento.mouse.y <= 150)
        {
          run_snake = 1;
        }
        if (evento.mouse.x >= 50 && evento.mouse.x <= 150 &&evento.mouse.y >= 100 &&
            evento.mouse.y <= 150)
        {
          run_hexspawn = 1;
        }
      }
    }

    if(run_snake == 1){
    	//rodar a cobrinha
    	printf("roda o snake\n");
    	sleep(30);
    	break;
    }else if(run_hexspawn){
    	//rodar hexswpawn
    	printf("roda o hexspawn\n");
    	sleep(30);
    	break;
    }

 
    // Limpamos a tela
    al_clear_to_color(al_map_rgb(0, 0, 0));
 
    // Colorimos o bitmap correspondente ao retângulo central,
    // com a cor condicionada ao conteúdo da flag na_area_central
    al_set_target_bitmap(hexspawn);
    if (!in_hexspawn)
    {
      al_clear_to_color(al_map_rgb(55, 255, 55));
    }
    else
    {
      al_clear_to_color(al_map_rgb(200, 255, 200));
    }

    al_set_target_bitmap(snake);
    if (!in_snake)
    {
      al_clear_to_color(al_map_rgb(55, 255, 55));
    }
    else
    {
      al_clear_to_color(al_map_rgb(200, 255, 200));
    }
 
    // Desenhamos os retângulos na tela
    al_set_target_bitmap(al_get_backbuffer(janela));


    al_draw_bitmap(hexspawn, 50, 100, 0);
    al_draw_bitmap(snake, 250, 100, 0);

    // Atualiza a tela
    al_flip_display();
  }
 
  // Desaloca os recursos utilizados na aplicação
  al_destroy_bitmap(snake);
  al_destroy_bitmap(hexspawn);
  al_destroy_display(janela);
  al_destroy_event_queue(fila_eventos);

  if(run_snake) snake();
 
  return 0;
}