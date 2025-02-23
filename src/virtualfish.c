// System Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for time() to seed srand()
#include <string.h>
#include <ctype.h> // for toupper()
#include <ncurses.h>

// Local Headers
#include "virtualfish.h"
#include "fish_graphic.h"

// Local Macros
#define TANK_BOUND_X 0
#define TANK_BOUND_Y 1
#define SPACE_KEY 32

// Gloabl Variables
static unsigned int seed = 0;
static unsigned int count = 0;
static unsigned int max = 0;
static unsigned char black_and_white = 0;
static unsigned char no_title_scr = 0;

static unsigned int tank_min_bounds[2] = {0,0};
static unsigned int tank_max_bounds[2] = {0,0};

int main(int argc, char** argv){

    set_flag_defaults();

    handle_flags(argc, argv);

    srand(seed);

    initscr();

    if(getmaxy(stdscr) < 12){
        endwin();
        printf("Terminal height is too short.\n");
        exit(0);
    }

    if(black_and_white == 0){
        if(has_colors() == 0){
            endwin();
            printf("Colors are not supported on this terminal.\n");
            exit(0);
        }
    
        start_color();
        init_color_pairs();
    }

    game_mode_on();

    init_env();

    if(no_title_scr == 0) title_screen();

    int update_code = 0;
    while((update_code = update()) == 0);

    endwin();

    return 0;
}

void set_flag_defaults(){

    black_and_white = 0;
    no_title_scr = 0;
    max = 10;
    count = 0;
    seed = time(NULL);

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
                seed = tmp_seed;
            }

            continue;

        }

        if(strcmp(argv[i], "-c") == 0 && (i + 1) != argc){
            
            int tmp_count = 0;
            
            if((tmp_count = atoi(argv[i + 1])) >= 0){
                count = tmp_count;
            }

            continue;

        }

        if(strcmp(argv[i], "-m") == 0 && (i + 1) != argc){
            
            int tmp_max = 0;
            if((tmp_max = atoi(argv[i + 1])) > 0){
                max = tmp_max;
            }

            continue;

        }

        if(strcmp(argv[i], "-bw") == 0){

            black_and_white = 1;
            continue;

        }

        if(strcmp(argv[i], "-nt") == 0){
            
            no_title_scr = 1;
            continue;
        
        }

    }

}

void init_env(){ 

    draw_box();
    draw_water();
    draw_sand(gen_sand());
    set_tank_bounds();
    refresh();

}

void title_screen(){

    const char* TITLE = "Virtualfish Redux";
    const char* MSG = "Press space to continue";

    WINDOW* title_win = newwin(8, 32, ((getmaxy(stdscr) / 2) - 4), ((getmaxx(stdscr) / 2) - 16));
    mvwaddnstr(title_win, 0, 8, TITLE, 18);

    if (black_and_white == 0) wattron(title_win, COLOR_PAIR(COLOR_CYAN));
    
    mvwaddnstr(title_win, 1, 1, FISH_GRAPHIC[0], 30);
    mvwaddnstr(title_win, 2, 1, FISH_GRAPHIC[1], 30);
    mvwaddnstr(title_win, 3, 1, FISH_GRAPHIC[2], 30);
    mvwaddnstr(title_win, 4, 1, FISH_GRAPHIC[3], 30);
    mvwaddnstr(title_win, 5, 1, FISH_GRAPHIC[4], 30);
    mvwaddnstr(title_win, 6, 1, FISH_GRAPHIC[5], 30);
    
    if(black_and_white == 0) wattroff(title_win, COLOR_PAIR(COLOR_CYAN)); 
    
    mvwaddnstr(title_win, 7, 5, MSG, 24);
    wrefresh(title_win);

    while(getch() != SPACE_KEY);

    wclear(title_win);
    wrefresh(title_win);

    delwin(title_win);

}

int update(){

    // read input

    char input = getch();

    // parse input

    if(input == ':' || input == ';'){

        int input_str_size = 128;
        char input_str[input_str_size];

        WINDOW* input_win = newwin(1, getmaxx(stdscr) - 2, getmaxy(stdscr) - 2, 1);
        waddch(input_win, ':');

        game_mode_off();
        wgetnstr(input_win, input_str, input_str_size - 1);
        game_mode_on();

        switch(parse_command(input_str, input_str_size)){

            case FISH:
                // todo: spawn fish
                waddnstr(input_win, "New Fish Spawned", 17);
                wrefresh(input_win);
                napms(750);
                break;
            case MAX:
                // todo: spawn max fish
                waddnstr(input_win, "Max Fish Spawned", 17);
                wrefresh(input_win);
                napms(750);
                break;
            case CLEAR:
                // todo: clear fish
                waddnstr(input_win, "Fish Cleared", 13);
                wrefresh(input_win);
                napms(750);
                break;
            case SEED:
                char parsed_seed[12] = {0};
                itocstr((signed int)seed, parsed_seed, 12);
                waddnstr(input_win, parsed_seed, 12);
                waddnstr(input_win, "\tPress space to resume game...", 31);
                while(!(wgetch(input_win) == SPACE_KEY));
                break;
            case QUIT:
                return 1;
            case _BLANK:
                break;
        }

        delwin(input_win);
        draw_sand(NULL);

    }

    // render updates

    refresh();

    return 0;

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

    if(black_and_white == FALSE) attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD);

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

    if(black_and_white == FALSE) attroff(COLOR_PAIR(COLOR_BLUE) | A_BOLD);

}

char* gen_sand(){

    char sand_chr[] = {'-', '.', '_'};
    int sand_offset_x = 2;
    
    unsigned int size = getmaxx(stdscr) - sand_offset_x;
    
    unsigned int sand_pattern_size = size + 1; // plus 1 for the null terminator

    char* sand_pattern = malloc(sizeof(char) * sand_pattern_size);

    int ch_index = 1;

    for(unsigned int i = 0; i < size; i++){

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

        sand_pattern[i] = sand_chr[ch_index];
        
    }

    sand_pattern[sand_pattern_size - 1] = '\0';

    return sand_pattern;

}

void draw_sand(char* sand_pat){

    static char* sand_pattern = NULL;

    if(sand_pat != NULL){
        free(sand_pattern);
        sand_pattern = sand_pat;
    }

    int sand_offset = 2;

    if(black_and_white == FALSE) attron(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);
    
    mvprintw((getmaxy(stdscr) - sand_offset), 1, "%s", sand_pattern);
    
    if(black_and_white == FALSE) attroff(COLOR_PAIR(COLOR_YELLOW) | A_BOLD);

}

void set_tank_bounds(){

    tank_min_bounds[TANK_BOUND_X] = 1;
    tank_min_bounds[TANK_BOUND_Y] = 2;

    tank_max_bounds[TANK_BOUND_X] = getmaxx(stdscr) - 2;
    tank_max_bounds[TANK_BOUND_Y] = getmaxy(stdscr) - 3;

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

void game_mode_on(){

    cbreak(); 
    noecho(); 
    timeout(10); 
    curs_set(0);

}

void game_mode_off(){

    nocbreak(); 
    echo(); 
    timeout(-1); 
    curs_set(1);

}

enum Command parse_command(char* parse_string, size_t parse_string_size){

    if(parse_string == NULL || parse_string_size == 0){
        return _BLANK;
    }

    parse_string[0] = toupper(parse_string[0]);

    switch(parse_string[0]){
        case 'F':
            return FISH;
        case 'M':
            return MAX;
        case 'C':
            return CLEAR;
        case 'S':
            return SEED;
        case 'Q':
            return QUIT;
    }

    return _BLANK;
}

void itocstr(int integer, char* string, size_t str_size){

    if(string == NULL || str_size == 0){
        return;
    }

    string[str_size - 1] = '\0';

    for(size_t index = 0; index < str_size - 1; index++){
        
        if(index == 0 && integer < 0){
            string[index] = '-';
            integer *= -1;
            continue;
        }

        string[index] = integer % 10 + '0';

        integer /= 10;

    }

    int start_index = 0;
    int end_index = str_size - 2;

    if(string[0] == '-') start_index++;

    while(end_index > start_index){

        // swaping algorithm from https://www.geeksforgeeks.org/swap-two-numbers-without-using-temporary-variable/#using-arithmetic-operators

        string[start_index] = string[start_index] + string[end_index];
        string[end_index] = string[start_index] - string[end_index];
        string[start_index] = string[start_index] - string[end_index];

        start_index++;
        end_index--;

    }

}

int rand_from_range(int min, int max){

    if(max < min){
        return 0;
    }

    int mod = (max + 1) - min;

    return rand() % mod + min;

}
