#ifndef VIRTUALFISH_H
#define VIRTUALFISH_H

enum Command {
    FISH,
    MAX,
    CLEAR,
    SEED,
};

/**
 * initializes global var Flag_Vals
 */
void set_flag_defaults();

/**
 * Handles argc & argv flags
 */
void handle_flags(int, char**);

/**
 * Initlizes tank grpahics
 */
void init_env();

/**
 * Runs title screen logic
 */
void title_screen();

/**
 * Runs the game loop and return 0 upon success
 */
int update();

/**
 * Draws a bolded box to stdscr
 */
void draw_box();

/**
 * Draws the water pattern for the tank
 */
void draw_water();

/**
 * Generates the sand pattern in an heap alloctaed c string and returns the pointer.
 */
char* gen_sand();

/**
 * Draws the sand pattern stored in the c string passed in and uses it statically, unless a new string is passed in.
 * 
 * NOTE: Whatever c string that is stored in the static varibale will have free() called upon it when a new one is passed.
 */
void draw_sand(char*);

/**
 * 
 */
void set_tank_bounds();

/**
 * Initilizes the 8 basic color pairs for color use
 */
void init_color_pairs();

/**
 * Turns on terminal modifers for the game mode
 */
void game_mode_on();

/**
 * Turns off terminal modifers for the game mode
 */
void game_mode_off();

enum Command parse_command(char* parse_string, int parse_string_size);

void itocstr(int integer, char* string, size_t str_size);

int rand_from_range(int min, int max);

#endif