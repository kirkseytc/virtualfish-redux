// C++ Libs
#include <iostream>

// C Libs
#include <ncurses.h>
// #include <pthread.h>

using std::cout;
using std::cin;
using std::endl;

#define INPUT_SCR_HEIGHT 3
#define RENDER_TANK 0x01
#define RENDER_INPUT 0x02
// #define RENDER_ 0x04
// #define RENDER_ 0x08
// #define RENDER_ 0x10
// #define RENDER_ 0x20
// #define RENDER_ 0x40
// #define RENDER_ 0x80

#define byte unsigned char

int main(int argc, char** argv){

    byte render_byte = 0x00;

    initscr();
    cbreak(); // give the programmer access to user input as the char is typed
    noecho(); // doesn't echo user inputs
    
    const int COLUMNS = getmaxx(stdscr);
    const int LINES = getmaxy(stdscr);

    WINDOW* tank_scr = newwin((LINES - INPUT_SCR_HEIGHT), (COLUMNS), 0, 0);
    
    WINDOW* input_scr = newwin(INPUT_SCR_HEIGHT, (COLUMNS), (LINES - INPUT_SCR_HEIGHT), 0);

    box(tank_scr, 0, 0);
    render_byte = render_byte | RENDER_TANK;

    box(input_scr, 0, 0);
    render_byte = render_byte | RENDER_INPUT;

    // render loop
    if(render_byte & RENDER_TANK){
        wnoutrefresh(tank_scr);
    }
    if(render_byte & RENDER_INPUT){
        wnoutrefresh(input_scr);
    }
    doupdate();

    char testarr[16] = {0};
    echo();
    mvwgetnstr(input_scr, 1, 1, testarr, 15);

    endwin();
    
    cout << testarr << endl;

    return 0;
}