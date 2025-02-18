// C++ Libs
#include <iostream>

// C Libs
#include <ncurses.h>
// #include <pthread.h>

using std::cout;
using std::cin;
using std::endl;

#define INPUT_SCR_HEIGHT 3

int render(WINDOW** window_list, uint size);

int main(int argc, char** argv){

    char testarr[16] = {0};
    const uint RENDER_LIST_SIZE = 2;
    WINDOW* render_list[RENDER_LIST_SIZE];

    initscr();
    cbreak(); // give the programmer access to user input as the char is typed
    noecho(); // doesn't echo user inputs
    
    const int COLUMNS = getmaxx(stdscr);
    const int LINES = getmaxy(stdscr);

    WINDOW* tank_scr = newwin((LINES - INPUT_SCR_HEIGHT), (COLUMNS), 0, 0);
    render_list[0] = tank_scr;
    WINDOW* input_scr = newwin(INPUT_SCR_HEIGHT, (COLUMNS), (LINES - INPUT_SCR_HEIGHT), 0);
    render_list[1] = input_scr;

    box(tank_scr, 0, 0);
    box(input_scr, 0, 0);

    render(render_list, RENDER_LIST_SIZE);

    echo();
    mvwgetnstr(input_scr, 1, 1, testarr, 15);

    endwin();
    
    cout << testarr << endl;

    return 0;
}

/**
 * Renders and list of windows starting from 0 to n in the list
 * 
 * @param window_list the list of WINDOW* to render
 * @param list_size the size of the list
 * @return the status code of the operation
 */
int render(WINDOW** window_list, uint list_size){

    int status_code;

    for(uint i = 0; i < list_size; i++){

        if((status_code = wnoutrefresh(window_list[i])) != OK){
            return ERR;
        }

    }

    if((status_code = doupdate()) != OK){
        return ERR;
    }

    return OK;
    
}