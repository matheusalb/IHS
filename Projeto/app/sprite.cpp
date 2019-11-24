// Os arquivos de cabeçalho
#include <allegro5/allegro.h>
 
#include <stdio.h>
 
// Atributos da tela
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
 
int main(void)
{
  ALLEGRO_DISPLAY *janela = NULL;
  ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
  ALLEGRO_BITMAP* boneco = NULL;
	ALLEGRO_BITMAP* left_bound = NULL;
	ALLEGRO_BITMAP* right_bound = NULL;
	ALLEGRO_BITMAP* up_bound = NULL;
	ALLEGRO_BITMAP* down_bound=NULL;
  // Flag que condicionará nosso looping
  int sair = 0;
 
  if (!al_init())
  {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }
 
  janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
  if (!janela)
  {
    fprintf(stderr, "Falha ao criar janela.\n");
    return -1;
  }
 
  // Configura o título da janela
  al_set_window_title(janela, "Rotinas de Mouse - www.rafaeltoledo.net");
 
 

 
  // Alocamos o retângulo central da tela
  boneco = al_create_bitmap( (int)LARGURA_TELA / 10,(int) ALTURA_TELA / 10);
  if (!boneco)
  {
    fprintf(stderr, "Falha ao criar bitmap.\n");
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

	if (!al_install_keyboard())
  {
    fprintf(stderr, "Falha ao inicializar o teclado.\n");
    return false;
  }

	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 


  int x=LARGURA_TELA / 2,y=ALTURA_TELA / 2;
  int tecla = 0;

	// Set to black background
  al_clear_to_color(al_map_rgb(0, 0, 0));

	// Draw red shape
	al_set_target_bitmap(boneco);
	al_clear_to_color(al_map_rgb(255, 0, 0));
	al_set_target_bitmap(al_get_backbuffer(janela));
  al_draw_bitmap(boneco, x,y, 0);
	





	al_flip_display();


  while (!sair)
  {
		
      while(!al_is_event_queue_empty(fila_eventos)){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {


            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                tecla = 1;
                break;
            case ALLEGRO_KEY_DOWN:
                tecla = 2;
                break;
            case ALLEGRO_KEY_LEFT:
                tecla = 3;
                break;
            case ALLEGRO_KEY_RIGHT:
                tecla = 4;
                break;
            }
        }
        else if(evento.type == ALLEGRO_EVENT_KEY_UP) tecla =0;

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sair = true;
        }

        if (tecla)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
        
            switch (tecla)
            {
                case 1:
                    y-=5;
                break;
                case 2:
                    y+=5;
                break;
                case 3:
                    x-=5;
                break;
                case 4:
                    x+=5;
                break;
            }

            al_set_target_bitmap(boneco);
            al_clear_to_color(al_map_rgb(255, 0, 0));

						al_set_target_bitmap(al_get_backbuffer(janela));
            al_draw_bitmap(boneco, x,y, 0);

						al_flip_display();

        }

	


    }




    
  }
 
  // Desaloca os recursos utilizados na aplicação
  al_destroy_bitmap(boneco);
  return 0;
}