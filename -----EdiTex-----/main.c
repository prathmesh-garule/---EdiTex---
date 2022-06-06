#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ncurses.h>

#include "UI.h"
#include "buffer.h"



int main(int argc, char const *argv[]){


    int fd, newfl = 0;
    int ht, wd;
    int srchflag = 0;
    int x = 0, y = 0, offY = 0, ch, xstate = 0, cpyi = 0, colr = 1;
    int i = 0;
    char str[LINEMAX], rstr[LINEMAX], filename[255], *srch, copybuf[LINEMAX]; /*max file name in linux is 255chars*/
    memset(filename, '\0', 255);
    memset(str, '\0', LINEMAX);
    memset(rstr, '\0', LINEMAX);
    memset(copybuf, '\0', LINEMAX);
    buffer *bf, *start, *temp, *temp2, *head;
    bf = (buffer *)malloc(sizeof(buffer));
    bufInit(bf);

// check eheather user Entered The filename 
// 

    if(argc == 2){
        /*checks if the filename exists already or not, else create new file*/
        strcpy(filename, argv[1]);
        if(fileexist(argv[1])){
            fd = open(argv[1], O_RDWR );
            bufLoad(fd, bf);
            close(fd);
            
        }
        else{
            newfl = 2;
            bf->line[0]='\n';
            bf->num_chars = 1;

        }
    }
    else if(argc == 1){

        /*./devtext i.e. open an empty file, initially the data is in buffer unless the file saved*/

        newfl = 1;
        bf->line[0]='\n';
        bf->num_chars = 1;
    }
    else{
        printf("USAGE: ./devtext <filename> or ./devtext\n");
        bufDestroy(bf);

        return 0;
    }



//ncurses here Functions 



curses_init();


getmaxyx(stdscr, ht, wd);
keypad(stdscr, true);

/* HELP MENU */
	
	attron(COLOR_PAIR(1));
	mvprintw(ht/2 - 7, wd/2 -  17, "Welcome to ---EdiTeXt---");


	attroff(COLOR_PAIR(1));


	attron(COLOR_PAIR(2));


	mvprintw(ht/2 - 5, wd/2 - 17, "CTRL+S OR F2 :  SAVE");
	mvprintw(ht/2 - 4, wd/2 - 17, "CTRL+A OR F3 :  SAVE AND QUIT");
	mvprintw(ht/2 - 3, wd/2 - 17, "CTRL+Q OR F4 :  QUIT WITHOUT SAVE");
	mvprintw(ht/2 - 2, wd/2 - 17, "CTRL+F OR F5 :  SEARCH");
	mvprintw(ht/2 - 1, wd/2 - 17, "CTRL+R OR F6 :  SEARCH AND REPLACE");
	mvprintw(ht/2 - 0, wd/2 - 17, "CTRL+X OR F7 :  CUT");
	mvprintw(ht/2 + 1, wd/2 - 17, "CTRL+C OR F8 :  COPY");
	mvprintw(ht/2 + 2, wd/2 - 17, "CTRL+V OR F9 :  PASTE");
	mvprintw(ht/2 + 3, wd/2 - 17, "CTRL+N       :  CHANGE TEXT COLOR");
	mvprintw(ht/2 + 4, wd/2 - 17, "CTRL+H OR F10 : SHOW HELP WINDOW");

	attroff(COLOR_PAIR(2));


	attron(COLOR_PAIR(1));

	mvprintw(ht/2 + 6, wd/2 - 17, "WINDOW SIZE: %3d X %3d", wd, ht); 
	mvprintw(ht/2 + 8, wd/2 - 17, "PRESS ANY KEY TO CONTINUE");


	attroff(COLOR_PAIR(1));

	y = 0, x = 0;
	move(y, x);
	refresh();
	getch();
	clear();



	start = bf;
	head = bf;
	temp2 = bf;
	loadwin(start, 0);
	attron(COLOR_PAIR(1));
	mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d | copy: \"%s\" |", filename, y, x, copybuf );
	move(y, x);
	attroff(COLOR_PAIR(1));
	refresh();
	move(0, 0);


    return 0;


}

