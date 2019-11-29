#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <time.h>
#include <stdlib.h>

#include <omp.h>

#include <algorithm>
#include <cstdio>
#include <vector>
#include <string>
#include <map> 
using namespace std;

// CORES
#define preto al_map_rgb(0, 0, 0)
#define branco al_map_rgb(255, 255, 255)
#define cinza al_map_rgb(150, 150, 150)
#define roxo_claro al_map_rgb(204, 204, 255)
#define vermelho al_map_rgb(255, 0, 0)

// TELA
ALLEGRO_DISPLAY *janela = NULL;
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

// IMAGENS
ALLEGRO_BITMAP *peaoPreto = NULL;
ALLEGRO_BITMAP *peaoBranco = NULL;
ALLEGRO_BITMAP *logo = NULL;
ALLEGRO_BITMAP *tabuleiro = NULL;
ALLEGRO_BITMAP *mini_tabuleiro = NULL;
ALLEGRO_BITMAP *press_play = NULL;
ALLEGRO_BITMAP *press_train = NULL;
ALLEGRO_BITMAP *arrow_left = NULL;
ALLEGRO_BITMAP *arrow_up = NULL;

// FILA DE EVENTOS
ALLEGRO_EVENT_QUEUE *filaEventos = NULL;

// FONTES
ALLEGRO_FONT *fonte_24;
ALLEGRO_FONT *fonte_36;

void iniciarAllegro(){
	al_init();
	// TELA
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	al_set_target_bitmap(al_get_backbuffer(janela));
	al_set_window_title(janela, "Hexapawn");
	// IMAGENS
	al_init_image_addon();
	peaoBranco = al_load_bitmap("./images/white.png");
	peaoPreto = al_load_bitmap("./images/black.png");
	logo = al_load_bitmap("./images/hexapawn-logo.png");
	tabuleiro = al_load_bitmap("./images/tabuleiro.png");
	mini_tabuleiro = al_load_bitmap("./images/tabuleiro100.png");
	press_play = al_load_bitmap("./images/play-text.png");
	press_train = al_load_bitmap("./images/train-text.png");
	arrow_left = al_load_bitmap("./images/left_arrow.png");
	arrow_up = al_load_bitmap("./images/up_arrow.png");
	// FILA DE EVENTOS
	al_install_keyboard();
	filaEventos = al_create_event_queue();
	al_register_event_source(filaEventos, al_get_display_event_source(janela));
	al_register_event_source(filaEventos, al_get_keyboard_event_source());
	// FONTES
	al_init_font_addon();
	al_init_ttf_addon();
	fonte_24 = al_load_font("./fonts/Retro_Gaming.ttf", 24, 0);
	fonte_36 = al_load_font("./fonts/Retro_Gaming.ttf", 36, 0);
}

// AUXILIAR DE DESENHO 32x32
void draw_amplified(ALLEGRO_BITMAP* bm, int x, int y, int sz = 4){
	al_draw_scaled_bitmap(bm, 0, 0, 32, 32, x, y, 32*sz, 32*sz, 0);
}

int scene = 0;

namespace HexaPawn {

	// LÓGICA DO JOGO

	const int tab_size = 3;
	map<string, vector<string> > learned;

	string random_element(vector<string> &choices){
		srand(time(NULL));
		int chosen = rand()%choices.size();
		return choices[chosen];
	}

	vector<string> learn_state(string state, int turn){
		vector<string> answer;
		string to;

		if(turn == 0){
			#pragma omp parallel for private(to)
			for(int i=tab_size; i<tab_size*tab_size; i++){
				if(state[i] == '1' && state[i-tab_size] == '0'){
					to = state;
					swap(to[i-tab_size], to[i]);
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
				if(i%tab_size >= 1 && state[i] == '1' && state[i-tab_size-1] == '2'){
					to = state;
					to[i] = '0';
					to[i-tab_size-1] = '1';
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
				if(i%tab_size <= 1 && state[i] == '1' && state[i-tab_size+1] == '2'){
					to = state;
					to[i] = '0';
					to[i-tab_size+1] = '1';
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
			}
		}
		else{
			#pragma omp parallel for private(to)
			for(int i=0; i<tab_size*(tab_size-1); i++){
				if(state[i] == '2' && state[i+tab_size] == '0'){
					to = state;
					swap(to[i], to[i+tab_size]);
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
				if(i%tab_size >= 1 && state[i] == '2' && state[i+tab_size-1] == '1'){
					to = state;
					to[i] = '0';
					to[i+tab_size-1] = '2';
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
				if(i%tab_size <= tab_size-2 && state[i] == '2' && state[i+tab_size+1] == '1'){
					to = state;
					to[i] = '0';
					to[i+tab_size+1] = '2';
					#pragma omp critical
					{
						answer.push_back(to);
					}
				}
			}
		}

		sort(answer.begin(), answer.end());
		reverse(answer.begin(), answer.end());
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

	// AUXILIAR DESENHO

	void drawMenu(){
		al_clear_to_color(roxo_claro);
		draw_amplified(peaoBranco, -45, 40, 9);
		draw_amplified(peaoPreto, 385, 40, 9);
		al_draw_scaled_bitmap(logo, 0, 0, 128, 32, -10, 325, 640, 160, 0);
		al_draw_scaled_bitmap(press_play, 0, 0, 72, 47, 180, 20, 268, 160, 0);
		al_draw_scaled_bitmap(press_train, 0, 0, 57, 47, 215, 180, 210, 160, 0);
		al_flip_display();
	}

	void drawTab(string tab = "000000000"){
		al_clear_to_color(branco);
		draw_amplified(tabuleiro, 0, 20, 12);
		for(int i=0; i<9; i++){
			if(tab[i] == '1') draw_amplified(peaoBranco, 20 + 108*(i%3), 40 + 108*(i/3));
			else if(tab[i] == '2') draw_amplified(peaoPreto, 20 + 108*(i%3), 40 + 108*(i/3));
		}
	}

	void draw_mini_tab(string tab, int x, int y){
		al_draw_bitmap(mini_tabuleiro, x, y, 0);
		for(int i=0; i<9; i++){
			if(tab[i] == '1') al_draw_bitmap(peaoBranco, x+1+(i%3)*33, y+1+(i/3)*33, 0);
			else if(tab[i] == '2') al_draw_bitmap(peaoPreto, x+1+(i%3)*33, y+1+(i/3)*33, 0);
		}
	}

	void draw_options(vector<string> moves){
		for(int i=0; i<moves.size(); i++){
			char str[3] = {};
			str[0] = i+'1';
			str[1] = ':';
			al_draw_text(fonte_24, preto, 425, 45+110*i, 0, str);
			draw_mini_tab(moves[i], 470, 10+110*i);
		}
	}

	void press_any_key(){
		al_draw_text(fonte_24, cinza, 20, 5, 0, "Press any key to continue");
	}

	// COMANDOS INPUT

	void get_random_input(){
		ALLEGRO_EVENT event;
		while(1){
			al_wait_for_event(filaEventos, &event);
			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) exit(0);
			if(event.type == ALLEGRO_EVENT_KEY_DOWN) break;
		}
	}

	void get_exit_option(){
		ALLEGRO_EVENT event;
		while(1){
			al_wait_for_event(filaEventos, &event);

			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) exit(0);
			
			if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE
					|| event.keyboard.keycode == ALLEGRO_KEY_2){
					scene = 0;
					break;
				}
				else if(event.keyboard.keycode == ALLEGRO_KEY_1) break;
			}
		}
	}

	int get_choice(int limit){

		int choice = 0;
		ALLEGRO_EVENT event;

		while(choice <= 0 || limit < choice){
			al_wait_for_event(filaEventos, &event);

			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) exit(0);

			if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				choice = event.keyboard.keycode - ALLEGRO_KEY_0;
				if(0 < choice && choice <= limit) break;
				else choice = 0;
			}
		}

		return choice-1;
	}

	bool confirm(){
		ALLEGRO_EVENT event;
		while(1){
			al_wait_for_event(filaEventos, &event);
			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) exit(0);
			if(event.type == ALLEGRO_EVENT_KEY_DOWN){
				return event.keyboard.keycode == ALLEGRO_KEY_1;
			}
		}
	}

	// JOGO

	bool play(string state = "222000111", int turn = 0){

		drawTab(state);

		vector<string> possibleMoves = learn_state(state, turn);
		if(is_final_state(state) || possibleMoves.size() == 0){
			if(turn == 0){
				al_draw_text(fonte_36, preto, 65, 400, 0, "YOU LOST :(");

				al_draw_text(fonte_24, preto, 440, 160, 0, "Press");
				al_draw_text(fonte_24, preto, 380, 190, 0, "1 - to try again");
				al_draw_text(fonte_24, preto, 380, 220, 0, "2 - to return to");
				al_draw_text(fonte_24, preto, 380, 250, 0, "menu");

				al_flip_display();
				get_exit_option();

				return 0;
			}
			else {
				al_draw_text(fonte_36, preto, 70, 400, 0, "YOU WON :)");

				al_draw_text(fonte_24, preto, 430, 380, 0, "Press");
				al_draw_text(fonte_24, preto, 350, 410, 0, "1 - to play again");
				al_draw_text(fonte_24, preto, 310, 440, 0, "2 - to return to menu");

				return 1;
			}
		}

		ALLEGRO_EVENT evento;

		if(turn == 0){

			al_draw_text(fonte_36, preto, 70, 400, 0, "YOUR TURN");

			draw_options(possibleMoves);
			al_flip_display();

			int choice = get_choice(possibleMoves.size());

			string chosen = possibleMoves[choice];
			return play(chosen, 1);
		}
		else{

			al_draw_text(fonte_36, preto, 70, 385, 0, "COMPUTER");
			al_draw_text(fonte_36, preto, 73, 425, 0, "DECIDING");
			press_any_key();

			if(!learned.count(state)) learned[state] = possibleMoves;
			else possibleMoves = learned[state];

			draw_options(possibleMoves);
			al_flip_display();

			get_random_input();

			string chosen = random_element(possibleMoves);
			bool erase = play(chosen, 0);

			if(erase == 1 && possibleMoves.size() > 1){

				al_draw_text(fonte_24, preto, 370, 40, 0, "Computer learned");
				al_draw_text(fonte_24, preto, 420, 70, 0, "not to try");
				draw_mini_tab(chosen, 445, 115);
				al_draw_text(fonte_24, preto, 455, 220, 0, "after");
				draw_mini_tab(state, 445, 260);

				al_flip_display();
				get_exit_option();

				learned[state] = remove_element(chosen, possibleMoves);
				erase = 0;
			}

			return erase;
		}
	}

	// TUTORIAL

	void renderInitial(){
		string state = "222000111";
		drawTab(state);
		draw_options(learn_state(state, 0));
		al_draw_text(fonte_36, preto, 70, 400, 0, "YOUR TURN");
	}

	void proceed(){
		press_any_key();
		al_flip_display();
		get_random_input();
	}

	void tutorial(){

		drawTab("222000111");
		al_draw_scaled_bitmap(arrow_left, 0, 0, 15, 15, 430, 0, 45, 45, 0);
		al_draw_text(fonte_24, vermelho, 470, 140, 0, "Hi :D");
		al_draw_text(fonte_24, vermelho, 420, 200, 0, "Welcome to");
		al_draw_text(fonte_24, preto, 430, 235, 0, "Hexapawn");
		proceed();

		drawTab("222000111");
		al_draw_text(fonte_24, vermelho, 390, 100, 0, "This is a game");
		al_draw_text(fonte_24, vermelho, 385, 135, 0, "based on chess");
		al_draw_text(fonte_24, vermelho, 370, 200, 0, "You have 3 pawns");
		al_draw_text(fonte_24, vermelho, 375, 235, 0, "against 3 others");
		al_draw_text(fonte_24, vermelho, 395, 270, 0, "controlled by");
		al_draw_text(fonte_24, vermelho, 398, 305, 0, "the computer");
		proceed();

		drawTab("010000000");
		al_draw_scaled_bitmap(arrow_left, 0, 0, 15, 15, 235, 90, 45, 45, 0);
		al_draw_text(fonte_24, vermelho, 382, 130, 0, "You can win the");
		al_draw_text(fonte_24, vermelho, 370, 165, 0, "game by reaching");
		al_draw_text(fonte_24, vermelho, 380, 200, 0, "the top line with");
		al_draw_text(fonte_24, vermelho, 413, 235, 0, "one of your");
		draw_amplified(peaoBranco, 465, 280, 2);
		proceed();

		drawTab("202121010");
		al_draw_text(fonte_24, vermelho, 390, 130, 0, "Or by making it");
		al_draw_text(fonte_24, vermelho, 396, 165, 0, "impossible for");
		al_draw_text(fonte_24, vermelho, 388, 200, 0, "your opponent");
		al_draw_text(fonte_24, vermelho, 440, 235, 0, "to move");
		proceed();

		drawTab("222000111");
		al_draw_text(fonte_24, vermelho, 390, 115, 0, "One more thing:");
		al_draw_text(fonte_24, vermelho, 398, 150, 0, "The computer");
		al_draw_text(fonte_24, vermelho, 402, 185, 0, "will learn how");
		al_draw_text(fonte_24, vermelho, 402, 220, 0, "to win as you");
		al_draw_text(fonte_24, vermelho, 390, 255, 0, "play agaisnt it");
		proceed();

		drawTab("222000111");
		al_draw_text(fonte_24, vermelho, 397, 120, 0, "But more on");
		al_draw_text(fonte_24, vermelho, 394, 155, 0, "that later ;)");
		al_draw_text(fonte_24, vermelho, 405, 220, 0, "Now to the");
		al_draw_text(fonte_24, vermelho, 390, 255, 0, "resources :)");
		proceed();

		renderInitial();
		al_draw_scaled_bitmap(arrow_left, 0, 0, 15, 15, 370, 340, 45, 45, 0);
		al_draw_text(fonte_24, vermelho, 430, 347, 0, "Current field");
		al_draw_text(fonte_24, vermelho, 340, 387, 0, "Shows the positions");
		al_draw_text(fonte_24, vermelho, 390, 417, 0, "of the pawns");
		proceed();

		renderInitial();
		al_draw_scaled_bitmap(arrow_left, 0, 0, 15, 15, 320, 400, 45, 45, 0);
		al_draw_text(fonte_24, vermelho, 380, 375, 0, "Indicates if you");
		al_draw_text(fonte_24, vermelho, 380, 405, 0, "or the computer");
		al_draw_text(fonte_24, vermelho, 420, 435, 0, "plays next");
		proceed();

		renderInitial();
		al_draw_scaled_bitmap(arrow_up, 0, 0, 15, 15, 500, 340, 45, 45, 0);
		al_draw_text(fonte_24, vermelho, 370, 385, 0, "Possible moves:");
		al_draw_text(fonte_24, vermelho, 360, 420, 0, "Pick one with the");
		al_draw_text(fonte_24, vermelho, 340, 445, 0, "corresponding key");
		proceed();

		renderInitial();
		al_draw_text(fonte_24, vermelho, 380, 360, 0, "Try and choose");
		al_draw_text(fonte_24, vermelho, 440, 390, 0, "1, 2 or 3");
		al_draw_text(fonte_24, vermelho, 380, 420, 0, "to start playing");
		al_flip_display();

		vector<string> possibleMoves = learn_state("222000111", 0);
		int choice = get_choice(possibleMoves.size());
		string chosen = possibleMoves[choice];
		play(chosen, 1);
	}

	// TRAIN

	bool cpu_play(bool show, string state = "222000111", int turn = 0){

		if(show) drawTab(state);

		vector<string> possibleMoves = learn_state(state, turn);
		if(is_final_state(state) || possibleMoves.size() == 0) return turn;

		if(turn == 0){
			bool any_victory = 0;
			#pragma omp parallel for
			for(int i=0; i<possibleMoves.size(); i++){
				if(cpu_play(show, possibleMoves[i], 1)){
					#pragma omp critical(victory)
					{
						any_victory = 1;
					}
				}
			}
			return any_victory;
		}
		else{
			learned[state] = possibleMoves;

			bool found_any_victory = 0;
			for(string chosen: possibleMoves){
				if(cpu_play(show, chosen, 0)){
					if(show){
						al_draw_text(fonte_24, preto, 370, 40, 0, "Computer learned");
						al_draw_text(fonte_24, preto, 420, 70, 0, "not to try");
						draw_mini_tab(chosen, 445, 115);
						al_draw_text(fonte_24, preto, 455, 220, 0, "after");
						draw_mini_tab(state, 445, 260);

						al_flip_display();
						get_random_input();
					}
					learned[state] = remove_element(chosen, learned[state]);
				}
				else found_any_victory = 1;
			}

			if(found_any_victory == 0) return 1;
			else return 0;
		}
	}

	bool trained = 0;
	bool train(bool show = 0){

		if(trained){
			al_clear_to_color(branco);
			al_draw_text(fonte_36, preto, 220, 200, 0 ,"ALREADY");
			al_draw_text(fonte_36, preto, 220, 240, 0 ,"TRAINED");
			press_any_key();
			al_flip_display();
		}
		else{

			al_clear_to_color(branco);
			al_draw_text(fonte_36, preto, 145, 70, 0 ,"Are you sure ?");
			al_draw_text(fonte_36, preto, 160, 150, 0 ,"This will make");
			al_draw_text(fonte_36, preto, 160, 190, 0 ,"the computer");
			al_draw_text(fonte_36, preto, 175, 230, 0 ,"unbeatable");

			al_draw_text(fonte_36, preto, 80, 310, 0 ,"Press 1 to confirm or");
			al_draw_text(fonte_36, preto, 40, 350, 0 ,"any other key to cancel");

			al_flip_display();

			if(confirm()){
				al_clear_to_color(branco);
				al_draw_text(fonte_36, preto, 220, 200, 0 ,"TRAINING");
				al_flip_display();

				cpu_play(show);
				trained = 1;

				al_clear_to_color(branco);
				press_any_key();
				al_draw_text(fonte_36, preto, 220, 200, 0, "FINISHED");
				al_flip_display();
			}
		}

		get_random_input();
	}
}


int main(void){

	iniciarAllegro();
	bool tutorialShown = 0;

	while(1){

		if(scene == 0) HexaPawn::drawMenu();

		int choice = HexaPawn::get_choice(2);

		if(scene == 0){
			switch(choice){
				case 0:
					if(tutorialShown == 0) scene = 1;
					else scene = 2;
				break;
				case 1:
					HexaPawn::train();
				break;
			}
		}
		
		if(scene == 1){
			tutorialShown = 1;
			scene = 2;
			HexaPawn::tutorial();
		}
		
		if(scene == 2){
			HexaPawn::play();
		}
	}

	return 0;
}
