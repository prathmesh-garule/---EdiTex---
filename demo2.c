///* pager functionality by Joseph Spainhour" <spainhou@bellsouth.net> */
//#include <ncurses.h>
//#include <stdlib.h>
//
//int main(int argc, char *argv[])
//{ 
//  int ch, prev, row, col;
//  prev = EOF;
//  FILE *fp;
//  int y, x;
//
//  if(argc != 2)
//  {
//    printf("Usage: %s <a c file name>\n", argv[0]);
//    exit(1);
//  }
//  fp = fopen(argv[1], "r");
//  if(fp == NULL)
//  {
//    perror("Cannot open input file");
//    exit(1);
//  }
//  initscr();				/* Start curses mode */
//  getmaxyx(stdscr, row, col);		/* find the boundaries of the screeen */
//  while((ch = fgetc(fp)) != EOF)	/* read the file till we reach the end */
//  {
//    getyx(stdscr, y, x);		/* get the current curser position */
//    if(y == (row - 1))			/* are we are at the end of the screen */
//    {
//      printw("<-Press Any Key->");	/* tell the user to press a key */
//      getch();
//      clear();				/* clear the screen */
//      move(0, 0);			/* start at the beginning of the screen */
//    }
//    if(prev == '/' && ch == '*')    	/* If it is / and * then only
//                                     	 * switch bold on */    
//    {
//      attron(A_BOLD);			/* cut bold on */
//      getyx(stdscr, y, x);		/* get the current curser position */
//      move(y, x - 1);			/* back up one space */
//      printw("%c%c", '/', ch); 		/* The actual printing is done here */
//    }
//    else
//      printw("%c", ch);
//    refresh();
//    if(prev == '*' && ch == '/')
//      attroff(A_BOLD);        		/* Switch it off once we got *
//                                 	 * and then / */
//    prev = ch;
//  }
//  endwin();                       	/* End curses mode */
//  fclose(fp);
//  return 0;
//}


// #include <ncurses.h>

// int main(int argc, char *argv[])
// {	initscr();			/* Start curses mode 		*/
// 	start_color();			/* Start color functionality	*/
	
// 	init_pair(1, COLOR_WHITE, COLOR_CYAN);
// 	printw("WELCOME TO EdiTeXt ");
// 	mvchgat(0, 0, -1,A_BLINK, 2, NULL);	
// 	/* 
// 	 * First two parameters specify the position at which to start 
// 	 * Third parameter number of characters to update. -1 means till 
// 	 * end of line
// 	 * Forth parameter is the normal attribute you wanted to give 
// 	 * to the charcter
// 	 * Fifth is the color index. It is the index given during init_pair()
// 	 * use 0 if you didn't want color
// 	 * Sixth one is always NULL 
// 	 */
// 	refresh();
//     	getch();
// 	endwin();			/* End curses mode		  */
// 	return 0;
// }



// #include <stdio.h>
// #include <ncurses.h>

// #define WIDTH 30
// #define HEIGHT 10 

// int startx = 0;
// int starty = 0;

// char *choices[] = { 
// 			"Choice 1",
// 			"Choice 2",
// 			"Choice 3",
// 			"Choice 4",
// 			"Exit",
// 		  };
// int n_choices = sizeof(choices) / sizeof(char *);
// void print_menu(WINDOW *menu_win, int highlight);

// int main()
// {	WINDOW *menu_win;
// 	int highlight = 1;
// 	int choice = 0;
// 	int c;

// 	initscr();
// 	clear();
// 	noecho();
// 	cbreak();	/* Line buffering disabled. pass on everything */
// 	startx = (80 - WIDTH) / 2;
// 	starty = (24 - HEIGHT) / 2;
		
// 	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
// 	keypad(menu_win, TRUE);
// 	mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
// 	refresh();
// 	print_menu(menu_win, highlight);
// 	while(1)
// 	{	c = wgetch(menu_win);
// 		switch(c)
// 		{	case KEY_UP:
// 				if(highlight == 1)
// 					highlight = n_choices;
// 				else
// 					--highlight;
// 				break;
// 			case KEY_DOWN:
// 				if(highlight == n_choices)
// 					highlight = 1;
// 				else 
// 					++highlight;
// 				break;
// 			case 10:
// 				choice = highlight;
// 				break;
// 			default:
// 				mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
// 				refresh();
// 				break;
// 		}
// 		print_menu(menu_win, highlight);
// 		if(choice != 0)	/* User did a choice come out of the infinite loop */
// 			break;
// 	}	
// 	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
// 	clrtoeol();
// 	refresh();
// 	endwin();
// 	return 0;
// }


// void print_menu(WINDOW *menu_win, int highlight)
// {
// 	int x, y, i;	

// 	x = 2;
// 	y = 2;
// 	box(menu_win, 0, 0);
// 	for(i = 0; i < n_choices; ++i)
// 	{	if(highlight == i + 1) /* High light the present choice */
// 		{	wattron(menu_win, A_REVERSE); 
// 			mvwprintw(menu_win, y, x, "%s", choices[i]);
// 			wattroff(menu_win, A_REVERSE);
// 		}
// 		else
// 			mvwprintw(menu_win, y, x, "%s", choices[i]);
// 		++y;
// 	}
// 	wrefresh(menu_win);
// }



#include <ncurses.h>

int main(int argc, char *argv[])
{	initscr();			/* Start curses mode 		*/
	start_color();			/* Start color functionality	*/
	
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	printw("A Big string which i didn't care to type fully ");
	mvchgat(0, 0, -1, A_BLINK, 1, NULL);	
	/* 
	 * First two parameters specify the position at which to start 
	 * Third parameter number of characters to update. -1 means till 
	 * end of line
	 * Forth parameter is the normal attribute you wanted to give 
	 * to the charcter
	 * Fifth is the color index. It is the index given during init_pair()
	 * use 0 if you didn't want color
	 * Sixth one is always NULL 
	 */
	refresh();
    	getch();
	endwin();			/* End curses mode		  */
	return 0;
}