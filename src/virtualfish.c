// System Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // for time() to seed srand()
#include <string.h>
#include <ctype.h> // for toupper()
#include <ncurses.h>

// Local Headers
#include "virtualfish.h"

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
static unsigned char rainbow_fish;

static unsigned int tank_win_start[2];
static unsigned int fish_max_pos[2];

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
        if(can_change_color() == 0){
            endwin();
            printf("Color mode is not supported on this terminal. Run with '-bw' flag.\n");
            exit(0);
        }
    
        start_color();

        if(COLORS < COLOR_TOTAL ){
            endwin();
            printf("Color mode is not supported on this terminal. Run with '-bw' flag.\n");
            exit(0);
        }

        init_color(COLOR_ORANGE, 1000, 647, 0);
        init_color(COLOR_BROWN, 627, 322, 176);
        init_color(COLOR_GREY, 439, 502, 565);

        init_pair((enum Color)WHITE, COLOR_WHITE, COLOR_BLACK);
        init_pair((enum Color)RED, COLOR_RED, COLOR_BLACK);
        init_pair((enum Color)ORANGE, COLOR_ORANGE, COLOR_BLACK);
        init_pair((enum Color)YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair((enum Color)GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair((enum Color)CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair((enum Color)BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair((enum Color)MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
        init_pair((enum Color)BROWN, COLOR_BROWN, COLOR_BLACK);
        init_pair((enum Color)GREY, COLOR_GREY, COLOR_BLACK);
        init_pair((enum Color)BLACK, COLOR_BLACK, COLOR_BLACK);

    }

    game_mode_on();

    init_env();
    
    if(no_title_scr == 0) title_screen();

    int update_code = 0;
    while((update_code = update()) == 0);

    endwin();

    switch(update_code){
        case 1:
            printf("Failed to allocate memory for fish array in 'update()'.\n");
            break;
        default:
            printf("Goodbye!\n");
    }

    return 0;
}

void set_flag_defaults(){

    max = 10;
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

        if(strcmp(argv[i], "-sm") == 0){
            
            start_count = max;

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

        if(strcmp(argv[i], "-rb") == 0){
            
            rainbow_fish = 1;
            continue;
        
        }

    }

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

void init_env(){ 

    draw_box();
    draw_water();
    draw_sand(gen_sand());
    set_tank_win_values();
    refresh();

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

    if(black_and_white == 0) attron(COLOR_PAIR(CYAN) | A_BOLD);

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

    if(black_and_white == 0) attroff(COLOR_PAIR(CYAN) | A_BOLD);

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

    if(black_and_white == 0) attron(COLOR_PAIR(YELLOW) | A_BOLD);
    
    mvprintw((getmaxy(stdscr) - sand_offset), 1, "%s", sand_pattern);
    
    if(black_and_white == 0) attroff(COLOR_PAIR(YELLOW) | A_BOLD);

}

void set_tank_win_values(){

    /**
     * 1 => 1 from box
    */ 
    tank_win_start[X] = 1;

    /**
     * 2 => 1 from box + 1 from water
     */
    tank_win_start[Y] = 2;

    /**
     * 3 => 1 from box + 2 for graphics
     */
    fish_max_pos[X] = (getmaxx(stdscr) - 1) - 3 - tank_win_start[X];

    /**
     * 2 => 1 from box + 1 from sand
     */
    fish_max_pos[Y] = (getmaxy(stdscr) - 1) - 2 - tank_win_start[Y];

}

void title_screen(){

    const char* TITLE = "Virtualfish Redux";
    const char* MSG = "Press space to continue";

    WINDOW* title_win = newwin(8, 32, ((getmaxy(stdscr) / 2) - 4), ((getmaxx(stdscr) / 2) - 16));
    mvwaddnstr(title_win, 0, 8, TITLE, 18); 
    mvwaddnstr(title_win, 7, 5, MSG, 24);

    enum Color title_fish_color = CYAN;

    while(getch() != SPACE_KEY){

        if(rainbow_fish == 1){
            title_fish_color = (title_fish_color++ % (COLOR_TOTAL - 4)) + 1;
        }
    
        if (black_and_white == 0) wattron(title_win, COLOR_PAIR(title_fish_color));
    
        for(size_t i = 0; i < 6; i++){
            mvwaddnstr(title_win, (i + 1), 7, TITLE_FISH_GRAPHIC[i], 20);
        }
        
        if(black_and_white == 0) wattroff(title_win, COLOR_PAIR(title_fish_color));
        wrefresh(title_win);
    }

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
            while(internal_count < start_count){
                fishes[internal_count] = create_fish();
                internal_count++;
            }
        }
    }

    // read input

    char input = getch();

    // parse input

    if(input == ':' || input == ';'){

        int input_str_size = 64;
        char input_str[input_str_size];

        WINDOW* input_win = derwin(stdscr, 1, (fish_max_pos[X] + 1) + 2, (fish_max_pos[Y] + 1) + tank_win_start[Y], tank_win_start[X]);
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

                while(internal_count < max){
                    fishes[internal_count] = create_fish();
                    internal_count++;
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

Fish create_fish(){

    Fish this;

    this.pos_x = (fish_max_pos[X] / 2) + ((rand() % 21) - 11);
    this.pos_y = (fish_max_pos[Y] / 2) + ((rand() % 15) - 7);

    this.direction.isNorth = rand() % 2;
    this.direction.isEast = rand() % 2;
    this.direction.onlyHorizontal = rand() % 2;

    this.counter = (rand() % 10) + 15;

    this.color = (rand() % COLOR_TOTAL - 3);

    return this;

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

Fish empty_fish(){
    static Fish empty = {};
    return empty;
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

void simulate(Fish* fishes, size_t fish_count){

    for(size_t i = 0; i < fish_count; i++){

        Fish* fish = fishes + i;

        if(rainbow_fish == 1 && (fish->counter % 4) == 0){
            fish->color = (fish->color++ % (COLOR_TOTAL - 4)) + 1;
        }

        if(fish->counter == 0){

            fish->direction = create_fish().direction;
            fish->counter = create_fish().counter;

        }

        if(fish->pos_x == 0){
            fish->direction.isEast = 1;
        }

        if(fish->pos_y == 0){
            fish->direction.isNorth = 0;
        }

        if(fish->pos_x == (int)fish_max_pos[X]){
            fish->direction.isEast = 0;
        }

        if(fish->pos_y == (int)fish_max_pos[Y]){
            fish->direction.isNorth = 1;
        }

        if(fish->counter){

            if(fish->direction.isEast){
                fish->pos_x++;
            } else {
                fish->pos_x--;
            }
    
            if(fish->direction.onlyHorizontal == 0){
                
                if(fish->direction.isNorth){
                    fish->pos_y--;
                } else {
                    fish->pos_y++;
                }
    
            }

        }
        
        simulate_OutOfBoundsHandle(fish);

        fish->counter--;

    }

}

void simulate_OutOfBoundsHandle(Fish* fish){

    integer_clamp(&fish->pos_x, 0, fish_max_pos[X]);
    integer_clamp(&fish->pos_y, 0, fish_max_pos[Y]);

}

int integer_clamp(int* num, int min, int max){

    if(*num < min){
        *num = min;
        return -1;
    }

    if(*num > max){
        *num = max;
        return 1;
    }

    return 0;
}

void render(Fish* fishes, size_t fish_count){

    WINDOW* tank = derwin(stdscr, fish_max_pos[Y] + 1, (fish_max_pos[X] + 1) + 2, tank_win_start[Y], tank_win_start[X]);
    wclear(tank);
    refresh();
    delwin(tank);

    // '&& i < max' might be optional, but just in case for now
    for(unsigned int i = 0; i < fish_count && i < max; i++){

        Fish fish = fishes[i];

        if(black_and_white == 0) attron(COLOR_PAIR(fish.color));

        char graphic[FISH_GRAPHIC_SIZE] = "<><";
        if(fish.direction.isEast) strcpy(graphic, "><>");

        mvaddnstr(fish.pos_y + tank_win_start[Y], fish.pos_x + tank_win_start[X], graphic, FISH_GRAPHIC_SIZE);
        
        if(black_and_white == 0) attroff(COLOR_PAIR(fish.color));

    }

    
    
}
