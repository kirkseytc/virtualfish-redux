#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#include "main.h"

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
    cbreak(); // give the programmer access to user input as the char is typed
    noecho(); // doesn't echo user inputs

    init_env();

    mvgetch(2, 1);

    endwin();

    return 0;
}

void init_env(){ // initalizing tank screen

    box(stdscr, 0, 0);
    water();
    sand();

}

void update_env(){



}

/**
 * Prints water pattern at the top of the tank
 */
void water(){ 

    const char WATER_CHR[] = {'-', '.', '_'};
    const int WATER_OFFSET = 3;
    const int WATER_START = 1;
    const int WATER_Y = 1;

    const int LENGTH = getmaxx(stdscr) - WATER_OFFSET;

    attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD);

    for(int counter = 0; counter < (LENGTH + 1); counter++){

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

/**
 * Prints a randomly generated sand pattern at the bottom of the tank
 */
void sand(){

    const char SAND_CHR[] = {'-', '.', '_'};
    const int SAND_OFFSET_X = 3;
    const int SAND_OFFSET_Y = 2;
    const int SAND_START = 1;

    const int SAND_Y = getmaxy(stdscr) - SAND_OFFSET_Y;
    const int LENGTH = getmaxx(stdscr) - SAND_OFFSET_X;
    
    int ch_index = 1;

    attron(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);

    for(int i = 0; i < (LENGTH + 1); i++){

        int x = i + SAND_START;

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

        mvaddch(SAND_Y, x, SAND_CHR[ch_index]);
    }

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

