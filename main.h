#ifndef MAIN_H
#define MAIN_H

struct sand_array {
    char* sand_pattern;
    unsigned int size;
};

void init_env();
void update_loop();
void water();
void gen_sand();
void draw_sand();
void init_color_pairs();
void game_on();
void game_off();

#endif