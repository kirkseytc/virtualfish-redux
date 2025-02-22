#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

#include "virtualfish.h"
#include "fish_graphic.h"

struct SAND_ARRAY Sand;
struct FLAG_DATA Flag_Vals;
struct TANK Tank_Vals;

int main(int argc, char** argv){

    set_flag_defaults();

    handle_flags(argc, argv);

    srand(Flag_Vals.seed);

    initscr();

    if(getmaxy(stdscr) < 12){
        endwin();
        printf("Terminal height is too short.\n");
        exit(0);
    }

    if(Flag_Vals.black_and_white == FALSE){

        if(has_colors() == FALSE){
            endwin();
            printf("Colors are not supported on this terminal.\n");
            exit(0);
        }
    
        start_color();
        init_color_pairs();

    }

    game_on();

    init_env();

    if(Flag_Vals.no_title == FALSE) { title_screen(); }

    update_loop();

    endwin();

    return 0;
}

void set_flag_defaults(){

    Flag_Vals.black_and_white = FALSE;
    Flag_Vals.no_title = FALSE;
    Flag_Vals.max = 10;
    Flag_Vals.count = 0;
    Flag_Vals.seed = time(NULL);

}

void handle_flags(int argc, char** argv){

    if(argc == 1){
        return;
    }

    for(int i = 1; i < argc; i++){

        if(argv[i][0] != '-'){
            continue;
        }

        if(strcmp(argv[i], "-s") == 0 && (i + 1) != argc){

            int tmp_seed = 0;
            
            if((tmp_seed = atoi(argv[i + 1])) != 0){
                Flag_Vals.seed = tmp_seed;
            }

            continue;

        }

        if(strcmp(argv[i], "-c") == 0 && (i + 1) != argc){
            
            int tmp_count = 0;
            
            if((tmp_count = atoi(argv[i + 1])) >= 0){
                Flag_Vals.count = tmp_count;
            }

            continue;

        }

        if(strcmp(argv[i], "-m") == 0 && (i + 1) != argc){
            
            int tmp_max = 0;
            if((tmp_max = atoi(argv[i + 1])) > 0){
                Flag_Vals.max = tmp_max;
            }

            continue;

        }

        if(strcmp(argv[i], "-bw") == 0){

            Flag_Vals.black_and_white = TRUE;
            continue;

        }

        if(strcmp(argv[i], "-nt") == 0){
            
            Flag_Vals.no_title = TRUE;
            continue;
        
        }

    }

}

void init_env(){ 

    draw_box();
    draw_water();
    gen_sand();
    draw_sand();
    set_tank_bounds();
    refresh();

}

void title_screen(){

    const char* TITLE = "Virtualfish Redux";
    const char* MSG = "Press space to continue";

    WINDOW* title_win = newwin(8, 32, ((getmaxy(stdscr) / 2) - 4), ((getmaxx(stdscr) / 2) - 16));
    mvwaddnstr(title_win, 0, 8, TITLE, 18);
    if(Flag_Vals.black_and_white == FALSE) { wattron(title_win, COLOR_PAIR(COLOR_CYAN)); }
    mvwaddnstr(title_win, 1, 1, FISH_GRAPHIC[0], 30);
    mvwaddnstr(title_win, 2, 1, FISH_GRAPHIC[1], 30);
    mvwaddnstr(title_win, 3, 1, FISH_GRAPHIC[2], 30);
    mvwaddnstr(title_win, 4, 1, FISH_GRAPHIC[3], 30);
    mvwaddnstr(title_win, 5, 1, FISH_GRAPHIC[4], 30);
    mvwaddnstr(title_win, 6, 1, FISH_GRAPHIC[5], 30);
    if(Flag_Vals.black_and_white == FALSE) { wattroff(title_win, COLOR_PAIR(COLOR_CYAN)); }
    mvwaddnstr(title_win, 7, 5, MSG, 24);
    wrefresh(title_win);

    while(getch() != 32);

    wclear(title_win);
    wrefresh(title_win);

    delwin(title_win);

}

void update_loop(){

    while(true){

        char input = getch();

        if(input == ':' || input == ';'){

            const int INPUT_SIZE = 128;

            char input_str[INPUT_SIZE];

            WINDOW* input_win = newwin(1, getmaxx(stdscr) - 2, getmaxy(stdscr) - 2, 1);
            waddch(input_win, ':');

            wrefresh(input_win);

            game_off();

            wgetnstr(input_win, input_str, INPUT_SIZE - 1);

            // TODO: String Parser

            game_on();

            delwin(input_win);

            draw_sand();

        }

        if(input == 'q'){
            break;
        }

        refresh();

    }

}

void draw_box(){

    attron(A_BOLD);
    box(stdscr, 0, 0);
    attroff(A_BOLD);

}

void draw_water(){ 

    const char WATER_CHR[] = {'-', '.', '_'};
    const int WATER_OFFSET = 2;
    const int WATER_START = 1;
    const int WATER_Y = 1;

    const int SIZE = getmaxx(stdscr) - WATER_OFFSET;

    if(Flag_Vals.black_and_white == FALSE) { attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD); }

    for(int counter = 0; counter < SIZE; counter++){

        int x = WATER_START + counter; // gets cursor x pos
        char ch = 0;

        switch(counter % 8){ // selects the character in the water pattern to draw
            case 0:
            case 1:
                ch = WATER_CHR[0];
                break;
            case 2:
            case 3:
            case 6:
            case 7:
                ch = WATER_CHR[1];
                break;
            case 4:
            case 5:
                ch = WATER_CHR[2];
                break;
        }

        mvaddch(WATER_Y, x, ch); // puts character to screen

    }

    if(Flag_Vals.black_and_white == FALSE) { attroff(COLOR_PAIR(COLOR_BLUE) | A_BOLD); }

}

void gen_sand(){

    const char SAND_CHR[] = {'-', '.', '_'};
    const int SAND_OFFSET_X = 2;
    
    const unsigned int SIZE = getmaxx(stdscr) - SAND_OFFSET_X;
    
    Sand.size = SIZE + 1; // plus 1 for the '\0'
    Sand.sand_pattern = malloc(sizeof(char) * Sand.size);

    int ch_index = 1;

    for(unsigned int i = 0; i < SIZE; i++){

        /*
            0 = stay same
            1 = go up
            2 = go down
        */

        int rnd = rand() % 3;

        switch(rnd){
            case 1:

                if(ch_index > 0){
                    ch_index--;
                }

                break;
            case 2:

                if(ch_index < 2){
                    ch_index++;
                }

                break;
        }

        Sand.sand_pattern[i] = SAND_CHR[ch_index];
        
    }

    Sand.sand_pattern[Sand.size - 1] = '\0';

}

void draw_sand(){

    const int SAND_OFFSET = 2;

    if(Flag_Vals.black_and_white == FALSE) { attron(COLOR_PAIR(COLOR_YELLOW) | A_BOLD); }
    
    mvprintw((getmaxy(stdscr) - SAND_OFFSET), 1, "%s", Sand.sand_pattern);
    
    if(Flag_Vals.black_and_white == FALSE) { attroff(COLOR_PAIR(COLOR_YELLOW) | A_BOLD); }

}

void set_tank_bounds(){

    Tank_Vals.min.x = 1;
    Tank_Vals.min.y = 2;

    Tank_Vals.max.x = getmaxx(stdscr) - 2;
    Tank_Vals.max.y = getmaxy(stdscr) - 3;

}

void init_color_pairs(){

    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);

}

void game_on(){

    cbreak(); 
    noecho(); 
    timeout(16); 
    curs_set(0);

}

void game_off(){

    nocbreak(); 
    echo(); 
    timeout(-1); 
    curs_set(1);

}

struct FISH* create_fish(int color){

    const int SPEED = 5;

    struct FISH* new_fish = malloc(sizeof(struct FISH));

    new_fish->color = color;

    new_fish->location.x = rand() % (Tank_Vals.max.x - Tank_Vals.min.x) + Tank_Vals.min.x;
    new_fish->location.y = rand() % (Tank_Vals.max.y - Tank_Vals.min.y) + Tank_Vals.min.y;

    new_fish->destination.x = rand() % (Tank_Vals.max.x - Tank_Vals.min.x) + Tank_Vals.min.x;
    new_fish->destination.y = rand() % (Tank_Vals.max.y - Tank_Vals.min.y) + Tank_Vals.min.y;

    new_fish->velocity.x = (float)((new_fish->destination.x - new_fish->location.x)/(SPEED));
    new_fish->velocity.y = (float)((new_fish->destination.y - new_fish->location.y)/(SPEED));

    return new_fish;

}
