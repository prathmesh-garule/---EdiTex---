#include "UI.h"
#include <ncurses.h>




void curses_init(){
    initscr();
    noecho();                   
    raw();
    curs_set(2);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); /*COLOR codes for ncurses*/
    init_pair(3,COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    // init_pair(3, COLOR_WHITE, COLOR_BLACK);



}

/*Renders the window from the first line i.e. Y = 0, and the "start" buffer pointer.*/
void loadwin(buffer *bf, int y){
    clear();
    attron(COLOR_PAIR(3));
    int x = 0;
    while(bf != NULL){
        mvprintw(y, x, "%s", bf->line);
        y++;
        bf = bf->next;
    }

    refresh();
}




void help_wind(int ht, int wd){
    attron(COLOR_PAIR(1));
	
	mvprintw(ht/2 -7, wd/2 -  17, "-------EdiTeXt-------");
	mvchgat(ht/2 -7, wd/2 -  17,-1,A_BLINK, 1,NULL);	
   // mvprintw(ht/2+5 , wd/2-17 , " ---Prathemsh Garule---");

	attroff(COLOR_PAIR(1));


	attron(COLOR_PAIR(2));

    

	// mvprintw(ht/2 - 5, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
	mvprintw(ht/2 - 4, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
	mvprintw(ht/2 - 3, wd/2 - 17, "CTRL+Q OR F4 :  QUIT ");
	mvprintw(ht/2 - 2, wd/2 - 17, "CTRL+F OR F5 :  SEARCH");
	mvprintw(ht/2 - 1, wd/2 - 17, "CTRL+R OR F6 :  SEARCH AND REPLACE");
	mvprintw(ht/2 - 0, wd/2 - 17, "CTRL+X OR F7 :  CUT");
	mvprintw(ht/2 + 1, wd/2 - 17, "CTRL+C OR F8 :  COPY");
	mvprintw(ht/2 + 2, wd/2 - 17, "CTRL+V OR F9 :  PASTE");
	
	mvprintw(ht/2 + 3, wd/2 - 17, "CTRL+H OR F10 : SHOW HELP WINDOW");

	attroff(COLOR_PAIR(2));


	attron(COLOR_PAIR(3));

	mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %3d X %3d", wd, ht); 
	mvprintw(ht/2 + 8, wd/2 - 17, "PRESS ANY KEY TO CONTINUE");

	attron(COLOR_PAIR(1));

	mvprintw(ht/2 + 10, wd/2 - 17, "______PRATHMESH______");

	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(3));


}


void help2(int ht, int wd){
    attron(COLOR_PAIR(1));


				mvprintw(ht/2 - 7, wd/2 -  17, "WELCOME TO Editext");
				attroff(COLOR_PAIR(1));
				attron(COLOR_PAIR(2));
				// mvprintw(ht/2 - 5, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
				mvprintw(ht/2 - 4, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
				mvprintw(ht/2 - 3, wd/2 - 17, "CTRL+Q OR F4 :  QUIT ");
				mvprintw(ht/2 - 2, wd/2 - 17, "CTRL+F OR F5 :  SEARCH");
				mvprintw(ht/2 - 1, wd/2 - 17, "CTRL+R OR F6 :  SEARCH AND REPLACE");
				mvprintw(ht/2 - 0, wd/2 - 17, "CTRL+X OR F7 :  CUT");
				mvprintw(ht/2 + 1, wd/2 - 17, "CTRL+C OR F8 :  COPY");
				mvprintw(ht/2 + 2, wd/2 - 17, "CTRL+V OR F9 :  PASTE");
				mvprintw(ht/2 + 3, wd/2 - 17, "CTRL+H OR F10 : SHOW HELP WINDOW");
				attroff(COLOR_PAIR(2));
				attron(COLOR_PAIR(1));
				mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %3d X %3d", wd, ht); 
				mvprintw(ht/2 + 8, wd/2 - 17, "PRESS ANY KEY TO CONTINUE");
				attroff(COLOR_PAIR(1));


}