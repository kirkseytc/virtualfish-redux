#ifndef VIRTUALFISH_H
#define VIRTUALFISH_H



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
    SIZE
};

typedef struct _FISH {

    float pos_x;
    float pos_y;

    int dest_pos_x;
    int dest_pos_y;

    int speed;

    float vel_x;
    float vel_y;

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
void set_tank_values();

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

int rand_from_range(int min, int max);

Fish create_fish();

Fish empty_fish();

void simulate(Fish* fishes, size_t fish_count);

Fish sim_oob(Fish fish);

void render(Fish* fishes, size_t fish_count);


#endif