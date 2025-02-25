#ifndef VIRTUALFISH_H
#define VIRTUALFISH_H

#define COMMAND_TOTAL 6
#define COLOR_TOTAL 8

enum Command {
    FISH,
    MAX,
    CLEAR,
    SEED,
    QUIT,
    _BLANK
};

enum Color {
    DEFAULT,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
};

typedef struct _DIRECTION {

    char isNorth;
    char isEast;
    char onlyHorizontal;

} Direction ;

typedef struct _FISH {

    int pos_x;
    int pos_y;

    Direction direction;

    unsigned int counter;

    enum Color color;

} Fish ;

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
void set_tank_win_values();

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

enum Command parse_command(char* parse_string, size_t parse_string_size);

void itocstr(int integer, char* string, size_t str_size);

Fish create_fish();

Fish empty_fish();

void simulate(Fish* fishes, size_t fish_count);

void simulate_OutOfBoundsHandle(Fish* fish);

Direction simulate_NextDirection(Direction direction);

void render(Fish* fishes, size_t fish_count);

int integer_clamp(int* num, int min, int max);

#endif