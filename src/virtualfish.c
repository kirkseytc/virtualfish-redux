const char VFR_VERSION[] = "1.2.3";

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
#define MAX_BUBBLES 4
#define MAX_FISH 10

// Gloabl Variables
static unsigned int seed;
static unsigned int start_count;
static unsigned int max;
static char black_and_white;
static char no_title_scr;
static char rainbow_fish;
static char colored_gravel;

static char crab_on;
static unsigned int crab_pos_x;
static char crab_direction;

static char castle_on;
static unsigned int castle_min_x;

static char volcano_on;
static unsigned int volcano_min_x;

static unsigned int tank_win_start[2];
static unsigned int fish_max_pos[2];

static Fish fishes[MAX_FISH];
static int fish_cnt;

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
        init_color(COLOR_VOL_BROWN, 627, 322, 176);
        init_color(COLOR_CAST_GREY, 439, 502, 565);
        init_color(COLOR_GRAV_ORANGE, 1000, 361, 0);
        init_color(COLOR_GRAV_PINK, 1000, 63, 941);
        init_color(COLOR_GRAV_BLUE, 0, 500, 1000);
        init_color(COLOR_GRAV_GREEN, 224, 1000, 78);
        init_color(COLOR_GRAV_YELLOW, 1000, 984, 0);

        init_pair((enum Color)WHITE, COLOR_WHITE, COLOR_BLACK);
        init_pair((enum Color)RED, COLOR_RED, COLOR_BLACK);
        init_pair((enum Color)ORANGE, COLOR_ORANGE, COLOR_BLACK);
        init_pair((enum Color)YELLOW, COLOR_YELLOW, COLOR_BLACK);
        init_pair((enum Color)GREEN, COLOR_GREEN, COLOR_BLACK);
        init_pair((enum Color)CYAN, COLOR_CYAN, COLOR_BLACK);
        init_pair((enum Color)BLUE, COLOR_BLUE, COLOR_BLACK);
        init_pair((enum Color)MAGENTA, COLOR_MAGENTA, COLOR_BLACK);

        init_pair((enum Ext_Color)VOL_BROWN, COLOR_VOL_BROWN, COLOR_BLACK);
        init_pair((enum Ext_Color)CAST_GREY, COLOR_CAST_GREY, COLOR_BLACK);
        init_pair((enum Ext_Color)GRAV_ORANGE, COLOR_GRAV_ORANGE, COLOR_BLACK);
        init_pair((enum Ext_Color)GRAV_PINK, COLOR_GRAV_PINK, COLOR_BLACK);
        init_pair((enum Ext_Color)GRAV_BLUE, COLOR_GRAV_BLUE, COLOR_BLACK);
        init_pair((enum Ext_Color)GRAV_GREEN, COLOR_GRAV_GREEN, COLOR_BLACK);
        init_pair((enum Ext_Color)GRAV_YELLOW, COLOR_GRAV_YELLOW, COLOR_BLACK);
        
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

        if(strcmp(argv[i], "-sm") == 0 || strcmp(argv[i], "--start-with-max") == 0){
            
            start_count = max;

            continue;

        }

        if(strcmp(argv[i], "-bw") == 0 || strcmp(argv[i], "--black-and-white") == 0){

            black_and_white = 1;
            continue;

        }

        if(strcmp(argv[i], "-nt") == 0 || strcmp(argv[i], "--no-title") == 0){
            
            no_title_scr = 1;
            continue;
        
        }

        if(strcmp(argv[i], "-rb") == 0 || strcmp(argv[i], "--rainbow") == 0){
            
            rainbow_fish = 1;
            continue;
        
        }
        
        if(strcmp(argv[i], "-cg") == 0 || strcmp(argv[i], "--colored-gravel") == 0){
            
            colored_gravel = 1;
            continue;
        
        }

        if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0){

            printf("Virtualfish Redux %s\n", VFR_VERSION);
            exit('V');

        }

        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){

            printf("Usage: virtualfish [OPTION(S)]\n");
            printf("an aquarium simulator\n\n");

            printf("  -h\t--help\t\t\tPrints the help screen\n");
            printf("  -v\t--version\t\tPrints the version\n");
            printf("  -bw\t--black-and-white\tStarts game in Black and White mode\n");
            printf("  -nt\t--no-title\t\tSkips the title screen\n");
            printf("  -rb\t--rainbow\t\tStarts game in Rainbow Fish mode. If the game is in Black and White mode, then nothing happens\n");
            printf("  -sm\t--start-with-max\tStarts the game with the max amount of fish spawned in\n");
            printf("  -cg\t--colored-gravel\tReplaces the sand with colored gravel\n\n");

            exit('H');

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

    draw_water();
    draw_sand(gen_sand());
    draw_box();
    refresh();

    // 1 => 1 from box 
    tank_win_start[X] = 1;

    // 2 => 1 from box + 1 from water
    tank_win_start[Y] = 2;
    
    // 3 => 1 from box + 2 for graphics
    fish_max_pos[X] = (getmaxx(stdscr) - 1) - 3 - tank_win_start[X];
    
    // 2 => 1 from box + 1 from sand
    fish_max_pos[Y] = (getmaxy(stdscr) - 1) - 2 - tank_win_start[Y];

    castle_min_x = (rand() % 11) + tank_win_start[X] + 1;
    volcano_min_x = fish_max_pos[X] - ((rand() % 11) + 17) + 2;
    crab_pos_x = rand() % (fish_max_pos[X] - 3);
    
    crab_direction = rand() % 2;

    if(start_count != fish_cnt){

        while(fish_cnt < start_count && fish_cnt < MAX_FISH){

            fishes[fish_cnt] = create_fish();
            fish_cnt += 1;

        }

    }

}

void draw_box(){

    attron(A_BOLD);
    box(stdscr, 0, 0);
    attroff(A_BOLD);

}

void draw_water(){ 

    static char skip_frame = 0;

    if(skip_frame == 0){
        skip_frame = 2;
    } else {
        skip_frame -= 1;
        return;
    }

    static int8_t frame_cnt = 3;

    static int size = 0;
    int x_offset = 1;

    if(size == 0){
        size = getmaxx(stdscr) - 3;
    }

    if(black_and_white == 0) attron(COLOR_PAIR(CYAN) | A_BOLD);

    char water_pattern[12] = {};

    switch(frame_cnt){
        case 0:
        case 11:
            strncpy(water_pattern, ".----.____", 11);
            break;
        case 1:
        case 10:
            strncpy(water_pattern, "_.----.___", 11);
            break;
        case 2:
        case 9:
            strncpy(water_pattern, "__.----.__", 11);
            break;
        case 3:
        case 8:
            strncpy(water_pattern, "___.----._", 11);
            break;
        case 4:
        case 7:
            strncpy(water_pattern, "____.----.", 11);
            break;
        case 5:
        case 6:
        case 16:
        case 17:
            strncpy(water_pattern, ".____.----", 11);
            break;

        case 12:
        case 21:
            strncpy(water_pattern, "----.____.", 11);
            break;
        case 13:
        case 20:
            strncpy(water_pattern, "---.____.-", 11);
            break;
        case 14:
        case 19:
            strncpy(water_pattern, "--.____.--", 11);
            break;
        case 15:
        case 18:
            strncpy(water_pattern, "-.____.---", 11);
            break;

    }
    
    frame_cnt++;
    frame_cnt = frame_cnt % 22;

    while(x_offset < size - 9){
        mvaddnstr(1, x_offset, water_pattern, 10);
        x_offset += 10;
    }

    mvaddnstr(1, x_offset, water_pattern, (size - x_offset + 2));

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

    if(black_and_white == 0){

        if(colored_gravel == 0) {

            attron(COLOR_PAIR(YELLOW) | A_BOLD);
    
            mvprintw((getmaxy(stdscr) - sand_offset), 1, "%s", sand_pattern);
        
            attroff(A_COLOR | A_BOLD);

            return;

        }

        int index = 0;
        int length = strlen(sand_pattern);

        static int color_lookup[64];
        
        if(color_lookup[0] == 0){ // create lookup table on first run

            for(int i = 0; i < 64; i++){

                int color_length = (rand() % 2) + 1;
                int color = (rand() % 5) + 10;

                if(i > 1){
                   
                    while(color == color_lookup[i - 1]){
                        color = (rand() % 5) + 10;
                    }

                }

                while(color_length > 0 && i < 64){

                    color_lookup[i] = color;

                    if(color_length != 1) i++;
                    color_length--;

                }

            }

        }

        while(index < length){

            attron(COLOR_PAIR(color_lookup[index % 64]));

            mvaddch((getmaxy(stdscr) - sand_offset), index + 1, '@');

            index++;
            
        }

        attroff(A_COLOR);

        return;

    }

    mvprintw((getmaxy(stdscr) - sand_offset), 1, "%s", sand_pattern);
    
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
            title_fish_color += 1;
            title_fish_color = (title_fish_color % (COLOR_TOTAL));
            if(title_fish_color == WHITE) title_fish_color = RED;
        }
    
        if (black_and_white == 0) wattron(title_win, COLOR_PAIR(title_fish_color));
    
        for(size_t i = 0; i < 6; i++){
            mvwaddnstr(title_win, (i + 1), 7, TITLE_FISH_GRAPHIC[i], 20);
        }
        
        if(black_and_white == 0) wattroff(title_win, A_COLOR);
        wrefresh(title_win);
    }

    wclear(title_win);
    wrefresh(title_win);

    delwin(title_win);

}

int update(){

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
                
                if(fish_cnt == max){
                    waddnstr(input_win, "Maximum amount of fish already exist.", 38);
                    wrefresh(input_win);
                    napms(750);
                    break;
                }

                fishes[fish_cnt] = create_fish();
                fish_cnt++;

                waddnstr(input_win, "Spawning Fish.", 15);
                wrefresh(input_win);
                napms(750);
                break;
            case MAX:
                
                if(fish_cnt == max){
                    waddnstr(input_win, "Maximum amount of fish already exist.", 38);
                    wrefresh(input_win);
                    napms(750);
                    break;
                }

                while(fish_cnt < max){
                    fishes[fish_cnt] = create_fish();
                    fish_cnt++;
                }

                waddnstr(input_win, "Spawning maximum fish", 22);
                wrefresh(input_win);
                napms(750);
                break;
            case CLEAR:

                fish_cnt = 0;

                waddnstr(input_win, "Clearing fish.", 15);
                wrefresh(input_win);
                napms(750);
                break;
            case SEED: {
                char parsed_seed[12] = {0};
                itocstr((signed int)seed, parsed_seed, 12);
                waddnstr(input_win, parsed_seed, 12);
                waddnstr(input_win, "\tPress space to resume game...", 31);
                while(!(wgetch(input_win) == SPACE_KEY));
                break;
            }
            case QUIT:

                return 127;

            case CRAB:

                if(crab_on == 0) {
                    crab_on = 1;
                    waddnstr(input_win, "Crab enabled.", 14);
                } else {
                    crab_on = 0;
                    waddnstr(input_win, "Crab disabled.", 15);
                }
                
                wrefresh(input_win);
                napms(750);
                break;

            case CASTLE:

                if(castle_on == 0){
                    castle_on = 1;
                    waddnstr(input_win, "Castle enabled.", 16);
                } else {
                    castle_on = 0;
                    waddnstr(input_win, "Castle disabled.", 17);
                }

                wrefresh(input_win);
                napms(750);
                break;

            case VOLCANO:

                if(volcano_on == 0){
                    volcano_on = 1;
                    waddnstr(input_win, "Volcano enabled.", 17);
                } else {
                    volcano_on = 0;
                    waddnstr(input_win, "Volcano disabled.", 18);
                }

                wrefresh(input_win);
                napms(750);
                break;
            case _BLANK:
                break;
        }

        delwin(input_win);

        draw_sand(NULL);

    }

    // simulation logic

    simulate();

    if(crab_on == 1){
        if(crab_direction == 1){ // heading east
            crab_pos_x++;
        } else {
            crab_pos_x--;
        }

        if(integer_clamp((int *)&crab_pos_x, tank_win_start[X], fish_max_pos[X] - 2)){

            if(crab_direction == 1){
                crab_direction = 0;
            } else {
                crab_direction = 1;
            }

        }
    }

    // render fish

    render();

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

    this.color = rand() % COLOR_TOTAL;

    if(rainbow_fish == 1 && this.color == WHITE) this.color = RED;

    return this;

}

enum Command parse_command(char* parse_string, size_t parse_string_size){

    if(parse_string == NULL || parse_string_size == 0){
        return _BLANK;
    }

    char tmp_char = toupper(parse_string[0]);

    if(tmp_char == 'C'){
        
        if(parse_string_size > 1){

            tmp_char = toupper(parse_string[1]);

            if(tmp_char == 'A') return CASTLE;
            if(tmp_char == 'R') return CRAB;
            if(tmp_char == 'L') return CLEAR;
            if(tmp_char != 0) return _BLANK;

        } 
        
    }

    if(tmp_char == 'F') return FISH;
    if(tmp_char == 'M') return MAX;
    if(tmp_char == 'S') return SEED;
    if(tmp_char == 'V') return VOLCANO;
    if(tmp_char == 'Q') return QUIT;

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

void simulate(){

    for(int8_t i = 0; i < fish_cnt; i++){

        Fish* fish = (fishes + i);

        if(rainbow_fish == 1 && (fish->counter % 4) == 0){
            fish->color += 1;
            fish->color = (fish->color % (COLOR_TOTAL));
            if(fish->color == WHITE) fish->color = RED;
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

void render(){

    WINDOW* tank = derwin(stdscr, fish_max_pos[Y] + 1, (fish_max_pos[X] + 1) + 2, tank_win_start[Y], tank_win_start[X]);
    wclear(tank);
    refresh();
    delwin(tank);

    // render bg layer
    if(volcano_on){

        if(black_and_white == 0) attron(COLOR_PAIR(ORANGE));

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 3, volcano_min_x, VOLCANO_GRAPHIC[0], 16);

        if(black_and_white == 0) attron(COLOR_PAIR(VOL_BROWN));

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 2, volcano_min_x, VOLCANO_GRAPHIC[1], 6);

        if(black_and_white == 0) attron(COLOR_PAIR(ORANGE));

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 2, volcano_min_x + 6, VOLCANO_GRAPHIC[1] + 6, 3);

        if(black_and_white == 0) attron(COLOR_PAIR(VOL_BROWN));

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 2, volcano_min_x + 9, VOLCANO_GRAPHIC[1] + 9, 7);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 1, volcano_min_x, VOLCANO_GRAPHIC[2], 16);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y], volcano_min_x, VOLCANO_GRAPHIC[3], 16);

        if(black_and_white == 0) attroff(A_COLOR);

    }

    if(castle_on){

        if(black_and_white == 0) attron(COLOR_PAIR(CAST_GREY));

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 5, castle_min_x, CASTLE_GRAPHIC[0], 12);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 4, castle_min_x, CASTLE_GRAPHIC[1], 12);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 3, castle_min_x, CASTLE_GRAPHIC[2], 12);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 2, castle_min_x, CASTLE_GRAPHIC[3], 12);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 1, castle_min_x, CASTLE_GRAPHIC[4], 12);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y], castle_min_x, CASTLE_GRAPHIC[5], 12);

        if(black_and_white == 0) attroff(A_COLOR);

    }

    draw_water();

    // render fish layer
    for(int8_t i = 0; i < fish_cnt; i++){

        static Fish fish; 
        fish = fishes[i];

        if(black_and_white == 0) attron(COLOR_PAIR(fish.color));

        char graphic[FISH_GRAPHIC_SIZE] = "<><";
        if(fish.direction.isEast) strncpy(graphic, "><>", FISH_GRAPHIC_SIZE);

        mvaddnstr(fish.pos_y + tank_win_start[Y], fish.pos_x + tank_win_start[X], graphic, FISH_GRAPHIC_SIZE);
        
        if(black_and_white == 0) attroff(A_COLOR);

    }

    // render fg
    if(crab_on){

        draw_sand(NULL);

        if(black_and_white == 0) attron(COLOR_PAIR(RED) | A_BOLD);

        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] - 1, crab_pos_x, CRAB_GRAPHIC[0], 7);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y], crab_pos_x, CRAB_GRAPHIC[1], 7);
        mvaddnstr(fish_max_pos[Y] + tank_win_start[Y] + 1, crab_pos_x, CRAB_GRAPHIC[2], 7);

        if(black_and_white == 0) attroff(A_COLOR | A_BOLD);

    }

    draw_box();

}
