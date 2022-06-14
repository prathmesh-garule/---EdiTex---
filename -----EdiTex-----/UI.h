#ifndef UI_H
#define UI_H
#include "buffer.h"
#include <ncurses.h>


void curses_init();
void loadwin(buffer *bf, int y);


void help_wind(int ht, int wd);
void help2(int ht, int wd);
#endif