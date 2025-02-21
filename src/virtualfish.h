#ifndef VIRTUALFISH_H
#define VIRTUALFISH_H

/**
 * Data structure to hold sand graphics data
 */
struct SAND_ARRAY {
    char* sand_pattern;
    unsigned int size;
};

/**
 * Data structure to hold data parsed by flag logic.
 */
struct FLAG_DATA {

    unsigned int seed;
    unsigned int count;
    unsigned int max;

    unsigned char black_and_white;
    unsigned char no_title;

};

/**
 * initializes global var Flag_Vals
 */
void set_flag_defaults();

/**
 * Handles argc & argv
 */
void handle_flags(int argc, char** argv);

/**
 * Initlizes tank grpahics
 */
void init_env();

/**
 * Runs title screen logic
 */
void title_screen();

/**
 * Runs the game loop
 */
void update_loop();

/**
 * Draws a bolded box to stdscr
 */
void draw_box();

/**
 * Draws the water pattern for the tank
 */
void draw_water();

/**
 * Generates the sand pattern and stores it in the global sand var
 */
void gen_sand();

/**
 * Draws the sand pattern stored in the global sand var
 */
void draw_sand();

/**
 * Initilizes the 8 basic color pairs for color use
 */
void init_color_pairs();

/**
 * Turns on terminal modifers for the game mode
 */
void game_on();

/**
 * Turns off terminal modifers for the game mode
 */
void game_off();

#endif