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
#define X 0
#define Y 1
#define SPACE_KEY 32
#define FISH_GRAPHIC_SIZE 4

// Gloabl Variables
static unsigned int seed;
static unsigned int start_count;
static unsigned int max;
static unsigned char black_and_white;
static unsigned char no_title_scr;

static unsigned int tank_win_size[2];
static unsigned int tank_win_start[2];
static unsigned int tank_win_end[2];

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

    switch(update_code){
        case 1:
            printf("Failed to allocate fish array in 'update()'.\n");
            break;
        default:
            printf("Goodbye!\n");
    }

    return 0;
}

void set_flag_defaults(){

    black_and_white = 0;
    no_title_scr = 0;
    max = 10;
    start_count = 0;
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
                if((unsigned int)tmp_count > max){
                    start_count = max;
                } else {
                    start_count = tmp_count;
                }
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
    set_tank_win_values();
    refresh();

}

void title_screen(){

    const char* TITLE = "Virtualfish Redux";
    const char* MSG = "Press space to continue";

    WINDOW* title_win = newwin(8, 32, ((getmaxy(stdscr) / 2) - 4), ((getmaxx(stdscr) / 2) - 16));
    mvwaddnstr(title_win, 0, 8, TITLE, 18);

    if (black_and_white == 0) wattron(title_win, COLOR_PAIR(COLOR_CYAN));

    mvwaddnstr(title_win, 1, 1, TITLE_FISH_GRAPHIC[0], 30);
    mvwaddnstr(title_win, 2, 1, TITLE_FISH_GRAPHIC[1], 30);
    mvwaddnstr(title_win, 3, 1, TITLE_FISH_GRAPHIC[2], 30);
    mvwaddnstr(title_win, 4, 1, TITLE_FISH_GRAPHIC[3], 30);
    mvwaddnstr(title_win, 5, 1, TITLE_FISH_GRAPHIC[4], 30);
    mvwaddnstr(title_win, 6, 1, TITLE_FISH_GRAPHIC[5], 30);
    
    if(black_and_white == 0) wattroff(title_win, COLOR_PAIR(COLOR_CYAN)); 
    
    mvwaddnstr(title_win, 7, 5, MSG, 24);
    wrefresh(title_win);

    while(getch() != SPACE_KEY);

    wclear(title_win);
    wrefresh(title_win);

    delwin(title_win);

}

int update(){

    static Fish* fishes = NULL;
    static unsigned int internal_count = 0;

    // allocate on first run
    if(fishes == NULL){
        if((fishes = malloc(sizeof(Fish) * max)) == NULL) return 1;

        if(internal_count != start_count){
            for(; internal_count < start_count; internal_count++){
                fishes[internal_count] = create_fish();
            }
        }
    }

    // read input

    char input = getch();

    // parse input

    if(input == ':' || input == ';'){

        int input_str_size = 64;
        char input_str[input_str_size];

        WINDOW* input_win = subwin(stdscr, 1, getmaxx(stdscr) - 2, getmaxy(stdscr) - 2, 1);
        wclear(input_win);
        waddch(input_win, ':');

        game_mode_off();
        wgetnstr(input_win, input_str, input_str_size - 1);
        game_mode_on();

        switch(parse_command(input_str, input_str_size)){

            case FISH:
                
                if(internal_count == max){
                    waddnstr(input_win, "Maximum amount of fish already exist.", 38);
                    wrefresh(input_win);
                    napms(750);
                    break;
                }

                fishes[internal_count] = create_fish();
                internal_count++;

                waddnstr(input_win, "Spawning Fish.", 15);
                wrefresh(input_win);
                napms(750);
                break;
            case MAX:
                
                if(internal_count == max){
                    waddnstr(input_win, "Maximum amount of fish already exist.", 38);
                    wrefresh(input_win);
                    napms(750);
                    break;
                }

                for(;internal_count < max; internal_count++){
                    fishes[internal_count] = create_fish();
                }

                waddnstr(input_win, "Spawning maximum fish", 22);
                wrefresh(input_win);
                napms(750);
                break;
            case CLEAR:
                
                for(unsigned int i; i < max; i++){
                    fishes[i] = empty_fish();
                }

                internal_count = 0;

                waddnstr(input_win, "Clearing fish.", 15);
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

                return INT32_MAX;
            case _BLANK:

                break;

        }

        delwin(input_win);
        draw_sand(NULL);

    }

    // simulation logic

    simulate(fishes, internal_count);

    // render fish

    render(fishes, internal_count);

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

void set_tank_win_values(){

    tank_win_start[X] = 1;
    tank_win_start[Y] = 2;

    tank_win_end[X] = (getmaxx(stdscr) - 1) - 1;
    tank_win_end[Y] = (getmaxy(stdscr) - 1) - 2;

    tank_win_size[X] = tank_win_end[X] - tank_win_start[X] + 1;
    tank_win_size[Y] = tank_win_end[Y] - tank_win_start[Y] + 1;


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
    timeout(100); 
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

Fish create_fish(){

    static Fish this;

    this.pos_x = ((tank_win_size[X] - FISH_GRAPHIC_SIZE) / 2) + tank_win_start[X];
    this.pos_y = (tank_win_size[Y] / 2) + tank_win_start[Y];

    this.direction.isNorth = rand() % 2;
    this.direction.isEast = rand() % 2;
    this.direction.onlyHorizontal = rand() % 2;

    this.counter = (rand() % 9) + 1;

    this.color = (enum Color)(rand() % COLOR_TOTAL);

    return this;

}

Fish empty_fish(){
    static Fish empty = {};
    return empty;
}

void simulate(Fish* fishes, size_t fish_count){

    for(size_t i = 0; i < fish_count; i++){

        Fish fish = fishes[i];

        fish = simulate_OutOfBoundsHandle(fish);

        if(fish.counter == 0){

            fish.direction = simulate_NextDirection(fish.direction);
            fish.counter = create_fish().counter;

        }

        if(fish.direction.isEast){
            fish.pos_x++;
        } else {
            fish.pos_x--;
        }

        if(fish.direction.onlyHorizontal == 0){
            
            if(fish.direction.isNorth){
                fish.pos_y--;
            } else {
                fish.pos_y++;
            }

        } 

        fish.counter--;


        fishes[i] = fish;

    }

}

Fish simulate_OutOfBoundsHandle(Fish fish){

    if(integer_clamp(&fish.pos_x, tank_win_start[X], tank_win_end[X])) fish.counter = 0;
    if(integer_clamp(&fish.pos_y, tank_win_start[Y], tank_win_end[Y])) fish.counter = 0;

    return fish;

}

Direction simulate_NextDirection(Direction direction){

    direction.isEast = rand() % 2; 
    direction.isNorth = rand() % 2;
    direction.onlyHorizontal = rand() % 2;

    return direction;

}

void render(Fish* fishes, size_t fish_count){

    WINDOW* tank = subwin(stdscr, tank_win_size[Y], tank_win_size[X], tank_win_start[Y], tank_win_start[X]);
    wclear(tank);

    // '&& i < max' might be optional, but just in case for now
    for(unsigned int i = 0; i < fish_count && i < max; i++){

        Fish fish = fishes[i];

        if(black_and_white == 0) wattron(tank, COLOR_PAIR(fish.color));

        char graphic[FISH_GRAPHIC_SIZE] = "<><";
        if(fish.direction.isEast) strcpy(graphic, "><>");

        mvwaddnstr(tank, fish.pos_y, fish.pos_x, graphic, FISH_GRAPHIC_SIZE);
        
        if(black_and_white == 0) wattroff(tank, COLOR_PAIR(fish.color));

    }

    refresh();

    delwin(tank);
    
}

int integer_clamp(int* num, int min, int max){

    if(*num > max){
        return 1;
        *num = max;
    }

    if(*num < min){
        return -1;
        *num = max;
    }

    return 0;
}
