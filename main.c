#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "main.h"
#include "fish_graphic.h"

struct SAND_ARRAY Sand;

int main(int argc, char** argv){

    srand(time(NULL)); // seeding rng

    initscr();

    if(has_colors() == FALSE){
        endwin();
        printf("Colors are not supported on this terminal.\n");
        exit(0);
    }

    start_color();
    init_color_pairs();

    game_on();

    init_env();

    title_screen();

    update_loop();

    endwin();

    return 0;
}

void init_env(){ 

    draw_box();
    draw_water();
    gen_sand();
    draw_sand();
    refresh();

}

void title_screen(){

    const char* TITLE = "Virtualfish Redux";
    const char* MSG = "Press any key to continue";

    WINDOW* title_win = newwin(8, 32, ((getmaxy(stdscr) / 2) - 4), ((getmaxx(stdscr) / 2) - 16));
    mvwaddnstr(title_win, 0, 8, TITLE, 18);
    wattron(title_win, COLOR_PAIR(COLOR_CYAN));
    mvwaddnstr(title_win, 1, 1, FISH_GRAPHIC[0], 30);
    mvwaddnstr(title_win, 2, 1, FISH_GRAPHIC[1], 30);
    mvwaddnstr(title_win, 3, 1, FISH_GRAPHIC[2], 30);
    mvwaddnstr(title_win, 4, 1, FISH_GRAPHIC[3], 30);
    mvwaddnstr(title_win, 5, 1, FISH_GRAPHIC[4], 30);
    mvwaddnstr(title_win, 6, 1, FISH_GRAPHIC[5], 30);
    wattroff(title_win, COLOR_PAIR(COLOR_CYAN));
    mvwaddnstr(title_win, 7, 4, MSG, 26);
    wrefresh(title_win);

    while(getch() == ERR);

    wclear(title_win);
    wrefresh(title_win);

    delwin(title_win);

}

void update_loop(){

    while(true){

        char input = getch();

        if(input == ':'){

            char cstr[128];

            WINDOW* input_win = newwin(1, getmaxx(stdscr) - 2, getmaxy(stdscr) - 2, 1);
            waddch(input_win, ':');

            wrefresh(input_win);

            game_off();

            wgetnstr(input_win, cstr, 127);

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

    attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD);

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

    attroff(COLOR_PAIR(COLOR_BLUE) | A_BOLD);

}

void gen_sand(){

    const char SAND_CHR[] = {'-', '.', '_'};
    const int SAND_OFFSET_X = 2;
    
    const unsigned int SIZE = getmaxx(stdscr) - SAND_OFFSET_X;
    
    Sand.size = SIZE + 1; // plus 1 for the '\0'
    Sand.sand_pattern = malloc(sizeof(char) * Sand.size);

    int ch_index = 1;

    for(int i = 0; i < SIZE; i++){

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

    attron(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
    mvprintw((getmaxy(stdscr) - SAND_OFFSET), 1, "%s", Sand.sand_pattern);
    attroff(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);

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