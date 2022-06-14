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
    int x = 0, y = 0, offY = 0, ch, xstate = 0, cpyi = 0;
    int i = 0;


    char str[LINEMAX], rstr[LINEMAX], filename[255], *srch, copybuf[LINEMAX]; /*max file name in linux is 255chars*/

	//initialy store '\0'

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
		//empty file 
        else{
            newfl = 2;
            bf->line[0]='\n';
            bf->num_chars = 1;

        }
    }

    else if(argc == 1){

        /*./Editext i.e. open an empty file, initially the data is in buffer unless the file saved*/

        newfl = 1;
        bf->line[0]='\n';
        bf->num_chars = 1;
    }
    else{
        printf("USAGE: ./Editext <filename> or ./Editext\n");
        bufDestroy(bf);

        return 0;
    }



//ncurses here Functions 



curses_init();


getmaxyx(stdscr, ht, wd);

keypad(stdscr, true);

// start_color();
// init_pair(1, COLOR_CYAN, COLOR_BLACK);

// 	mvprintw(ht,wd,"Prathmesh@");
	// mvchgat(0, 0, -1, A_BLINK, 1, NULL);


/* HELP MENU */
	help_wind(ht,wd);
	
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


    while((ch = getch())){
	
		switch (ch){


	//KEY TO IMPLEMENT :

			//1.UP /
			//2.DOWN/
			//3.LEFT/
			//4.RIGHT/
			//5.BACKSPACE/
			//8.END /
			//9.NEXT PAGE/
			//10.PAGE UP/
			// 11.ENTER/


/*          FUNCTIONS   */

			//---->SEARCH /
			//---->REPLACE /
			//---->CUT /
			//---->COPY /
			//---->PASTE /
			//---->SAVE /
			
			//---->EXIT /
			//---->HELP /




			case KEY_UP: /*up arrow*/
				if(y > 0 && bf != NULL && bf->prev != NULL){
					bf = bf->prev;
					if(x >= bf->num_chars){
						x = bf->num_chars -1;	
						move(--y, x);
					}
					else{
						move(--y, x);
					}
				}
				else  if( y == 0 && start->prev != NULL && bf->prev != NULL){
					start = start->prev;
					bf = bf->prev;
					loadwin(start, 0);
					move(y, x);
				}
				break;



			case KEY_DOWN: /*down arrow*/
				if(y < ht - 2 && bf != NULL){
					if(bf->next != NULL){
						bf = bf->next;
						if(x >= bf->num_chars){
							if(bf->num_chars  > 0){
								x = bf->num_chars - 1;								
							}
							else{
								x = 0;
							}
							move(++y, x);
						}
						else{
							move(++y, x);
						}
					}
				}
				else if( y == ht - 2 && bf->next != NULL && start->next != NULL){
					start = start->next;
					bf = bf->next;
					loadwin(start, 0);
					move(y, x);
				}
				break;


			case KEY_LEFT: /*left arrow*/
				if(x > 0){
					move(y, --x);
				}
				break;





			case KEY_RIGHT: /*right arrow*/
				if(x < LINEMAX - 1 && x < bf->num_chars - 1){
					move(y, ++x);
				}
				break;





			case KEY_BACKSPACE: /*BACKSPACE key*/


				bf->curloc = x;
				if(bf->prev != NULL){
					temp = bf->prev;
					
				}
				if(x == 0 && bf == head){
					move(0, 0);
				}

				else if(x > 0 && x < bf->num_chars){

					//memmove (where to move , what to move , how many)
					memmove((bf->line + x - 1), (bf->line + x), bf->num_chars - x);
					(bf->num_chars)--;
					x--;
					bf->curloc = x;
					loadwin(start, 0);
					move(y, x);
					refresh();


				}




// we will try to add whole line onto previos:

				else if(x == 0 && bf->prev != NULL && ( bf->prev->num_chars + bf->num_chars - 1 < LINEMAX)){
					if(bf == start ){
						start = start->prev;
					}
					if(bf == temp2){
							temp2 = bf->prev;
					}
					if(bf->prev->num_chars == 0){
						bf->prev->line[0] = '\n';
						bf->prev->num_chars = 1;
					}
					memmove((bf->prev->line + bf->prev->num_chars - 1), bf->line, (bf->num_chars - 0));



// now just ignore the whole thing after this 
//ignore current buffer
					bf->prev->next = bf->next;

					if(bf->next != NULL){
						bf->next->prev = bf->prev;						
					}

					x = bf->num_chars - 1;


					if(bf->prev->num_chars == 1 && bf->num_chars == 1){
						bf->prev->num_chars = 1;
					}
					else{
						bf->prev->num_chars = bf->prev->num_chars + bf->num_chars - 1;
					}
			
					free(bf->line);
					free(bf);

					bf = temp;


					if(bf->next != NULL){

						bufDecr(bf->next, 1);
						
					}



					if(y == 0){

						offY--;

						loadwin(start, 0);
						move(y, bf->curloc = x = bf->num_chars - x - 1);

					}
					else{


						y--;
						move(y, bf->curloc = x = bf->num_chars - x - 1);
						loadwin(start, 0);	
					}
				}
				break;




				case KEY_NPAGE: /*next page : page down*/
				for(i = 0; i < ht - 2 && temp2 != NULL && temp2->next != NULL; i++){
					temp2 = temp2->next;
				}
				//jaise hi temp becomes NULL We should Move To Next page
				i = 0; 
				start = temp2;
				bf = temp2;
				y = x = 0;
				move(y, x);
				loadwin(start, 0);
				break;




			case KEY_PPAGE: /*previous page : page up*/
				for(i = 0; i < ht - 2 && temp2 != NULL && temp2->prev != NULL; i++){
					temp2 = temp2->prev;
				}
				i = 0; 
				start = temp2;
				bf = temp2;
				y = x = 0;
				move(y, x);
				loadwin(start, 0);
				break;
		


			case '\n': /*enter is pressed*/
				bf->curloc = x;
				lineInsert(bf, bf->curloc, ch);

				if(x < bf->num_chars -1){
					bufInsert(bf);
					//if user prsses ENter Inbetween Th line
					//remaining should Move to Next line

					memmove(bf->next->line, (bf->line + x + 1), bf->num_chars - x + 1);
					memset((bf->line + x + 1), '\0', bf->num_chars - x);
					bf->next->num_chars = bf->num_chars - x - 1;

					bf->num_chars = x + 1;

					bf = bf->next;

					if(y < ht - 2)
						move(++y, x = 0);
					else{
						if(start->next != NULL){
							start = start->next;
							offY++;
						}

						move(y = ht - 2, x = 0);
					}
					loadwin(start, 0);


				}
				//End of Current Window
				else if(x == bf->num_chars || x == bf->num_chars - 1){
					bufInsert(bf);
					clear();
					loadwin(start, 0);					
					bf = bf->next;
					if(y < ht - 2)
						move(++y, x = 0);
					else{
						if(start->next != NULL){
							start = start->next;
							offY++;
						}

						move(y = ht - 2, x = 0);
					}
					loadwin(start, 0);
				}
				break;



			case 17:/*CTRL-Q there is no break statement.*/
			case KEY_F(4):
				endwin();
				close(fd);
/*				printf("%s\n", filename);
				bufPrintAll(head);*/
				bufDestroy(head);
				return 0;
				break;

		





		//Functions :::::::::

			case 8: /*ctrl-h help screen*/
			case KEY_F(10):
				clear();
				help2(ht,wd);
				move(y, x);
				curs_set(0);
				getch();
				curs_set(2);
				loadwin(start, 0);
				break;








		    case 6: /*CTRL-F*/
			case KEY_F(5): /*search*/
				temp2 = head;
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				mvprintw(ht - 1, 0, "Enter Search String: ");
				echo();
				mvscanw(ht - 1, strlen("Enter Search String: "), "%[^\n]s", str);
				refresh();
				noecho();
				while(temp2->next != NULL){
					x = 0;
					while(x >= 0 && x < temp2->num_chars - 1){

						srch = strstr(temp2->line + x, str);
						if(srch != NULL){
							x = temp2->curloc = srch - temp2->line;
							bf = temp2;
							attroff(COLOR_PAIR(1));
							if(temp2->cur_line - start->cur_line >= 0 && temp2->cur_line - start->cur_line < ht - 1){
								y = temp2->cur_line - start->cur_line ;
								move(y, x);
							}
							else{
								y = 0;
								start = temp2;
								loadwin(start, 0);
								
							}
							attron(COLOR_PAIR(1));
							move(ht - 1, 0);
							clrtoeol();
							mvprintw(ht - 1, 0, "ENTER : search next ANY: exit search");
							move(y , x);
							refresh();
							if((ch = getch()) != ERR){
								if(ch == '\n'){
									x++;
								}
								else{
									srchflag = 1;
									attroff(COLOR_PAIR(1));
									move(y, x);
									break;
								}
							}

						}
						else{
							x++;
						}
					}
					if(srchflag == 1)
						break;
					temp2 = temp2->next;
				}
				if(srchflag == 0){
					attron(COLOR_PAIR(1));
					move(ht - 1, 0);
					clrtoeol();
					mvprintw(ht - 1, 0, "END OF SEARCH");
					attroff(COLOR_PAIR(1));
					refresh();
					ch = getch();
				}
				srchflag = 0;
				break;








			case 18: /*CTRL-R*/
			case KEY_F(6): /*search replace*/
				temp2 = head;
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "Enter Search String: ");
				refresh();
				mvscanw(ht - 1, strlen("Enter Search String: "), "%[^\n]s", str);
				clrtoeol();
				mvprintw(ht - 1, 0, "Enter Replace String: ");
				refresh();
				mvscanw(ht - 1, strlen("Enter Replace String: "), "%[^\n]s", rstr);				
				noecho();
				while(temp2->next != NULL){
					x = 0;
					while(x >= 0 && x < temp2->num_chars - 1){

						srch = strstr(temp2->line + x, str);
						if(srch != NULL){
							x = temp2->curloc = srch - temp2->line;
							bf = temp2;
							attroff(COLOR_PAIR(1));
							if(temp2->cur_line - start->cur_line >= 0 && temp2->cur_line - start->cur_line < ht - 1){
								y = temp2->cur_line - start->cur_line ;
								move(y, x);
							}
							else{
								y = 0;
								start = temp2;
								loadwin(start, 0);
								
							}
							attron(COLOR_PAIR(1));
							move(ht - 1, 0);
							clrtoeol();
							mvprintw(ht - 1, 0, "ENTER: search next CTRL+R OR F(6): replace ANY: exit");
							move(y , x);
							refresh();
							if((ch = getch())){
								if(ch == '\n'){
									x++;
								}
								else if((ch == KEY_F(6) || ch == 18) && (x + strlen(rstr) - 1 < LINEMAX)){
									for(i = 0; i < strlen(str); i++){
										memmove((bf->line + x), (bf->line + x + 1), bf->num_chars - x - 1);
										(bf->num_chars)--;
										
									}
									i = 0;
									for( i = 0; i < strlen(rstr); i++){
										lineInsert(bf, x + i, rstr[i]);
									}
									i = 0;
									attroff(COLOR_PAIR(1));
									loadwin(start, 0);
									attron(COLOR_PAIR(1));
								}
								else{
									srchflag = 1;
									attroff(COLOR_PAIR(1));
									move(y, x);
									break;
								}
							}

						}
						else{
							x++;
						}
					}
					if(srchflag == 1)
						break;
					temp2 = temp2->next;
				}
				if(srchflag == 0){
					attron(COLOR_PAIR(1));
					move(ht - 1, 0);
					clrtoeol();
					mvprintw(ht - 1, 0, "END OF SEARCH");
					attroff(COLOR_PAIR(1));
					refresh();
					ch = getch();
				}
				srchflag = 0;
				break;








			case 24: /*CTRL-X*/
			case KEY_F(7): /*cut ctrl+X*/
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "RIGHT: select LEFT: deselect CTRL+X: cut selection ANY: cancel");
				attroff(COLOR_PAIR(1));
				refresh();
				noecho();
				move(y, x);
				cpyi = 0;
				xstate = x;
				memset(copybuf, '\0', LINEMAX);
				while((ch = getch()) != ERR){
					if(ch == KEY_RIGHT && x < LINEMAX - 1 && x < bf->num_chars - 1){
						copybuf[cpyi++] = bf->line[x];
						addch(bf->line[x] | A_STANDOUT);
						move(y, x++);
					}
					else if((ch == KEY_LEFT) && x >= 0 && x > xstate){
						
						move(y, --x);						
						copybuf[--cpyi] = '\0';
						addch(bf->line[x] | A_NORMAL);

					}
					else if(ch == 24 || ch == KEY_F(7)){
						x = xstate;
						for(i = 0; i < strlen(copybuf); i++){
							memmove((bf->line + x), (bf->line + x + 1), bf->num_chars - x - 1);
							(bf->num_chars)--;
										
						}
						i = 0;
						loadwin(start, 0);
						move(y, x);
						break;
					}
					else {
						loadwin(start, 0);
						memset(copybuf, '\0', LINEMAX);
						move(y, x = xstate);
						break;

					}
				move(ht - 1, 0);
				clrtoeol();
				attron(COLOR_PAIR(1));
				mvprintw(ht - 1, 0, " \"%s\" | RIGHT: select LEFT: deselect CTRL+X: cut selection ANY: cancel", copybuf);
				attroff(COLOR_PAIR(1));
				refresh();
				move(y, x);
				}			

				move(y, x);
				break;


			case 3:
			case KEY_F(8): /*copy ctrl+C*/
				attron(COLOR_PAIR(1));
				move(ht - 1, 0);
				clrtoeol();
				echo();
				mvprintw(ht - 1, 0, "RIGHT: select LEFT: deselect CTRL+C: copy selection ANY: cancel");
				attroff(COLOR_PAIR(1));
				refresh();
				noecho();
				move(y, x);
				cpyi = 0;
				xstate = x;
				memset(copybuf, '\0', LINEMAX);
				while((ch = getch()) != ERR){
					if(ch == KEY_RIGHT && x < LINEMAX - 1 && x < bf->num_chars - 1){
						copybuf[cpyi++] = bf->line[x];
						addch(bf->line[x] | A_STANDOUT);
						move(y, x++);
					}
					else if((ch == KEY_LEFT) && x >= 0 && x > xstate){
						
						move(y, --x);						
						copybuf[--cpyi] = '\0';
						addch(bf->line[x] | A_NORMAL);

					}
					else if(ch == 3 || ch == KEY_F(8)){
						x = xstate;
						loadwin(start, 0);
						move(y, x);
						break;
					}
					else {
						loadwin(start, 0);
						memset(copybuf, '\0', LINEMAX);
						move(y, x = xstate);
						break;


					}
				move(ht - 1, 0);
				clrtoeol();
				attron(COLOR_PAIR(1));
				mvprintw(ht - 1, 0, " \"%s\" | RIGHT: select LEFT: deselect CTRL+C: copy selection ANY: cancel", copybuf);
				attroff(COLOR_PAIR(1));
				refresh();
				move(y, x);
				}				
				move(y, x);
				break;






			case 22: /*CTRL-V*/
			case KEY_F(9): /*paste ctrl+V*/
				if(strlen(copybuf) != 0 && copybuf[0] != '\0' && x + strlen(copybuf) - 1 < LINEMAX){
					for(i = 0; i < strlen(copybuf); i++){
						lineInsert(bf, x + i, copybuf[i]);
					}
					
				}
				i = 0;
				move(y, bf->curloc = x = x + strlen(copybuf));
				loadwin(start, 0);			
				break;


			case 19: /*ctrl+S*/
			case KEY_F(2): /*save*/
				move(ht -1, 0);
				clrtoeol();
				if(newfl == 1){
					attron(COLOR_PAIR(1));
					mvprintw(ht - 1, 0, "Enter file Name: ");
					echo();
					mvscanw(ht - 1, strlen("Enter file Name: "), "%[^\n]s", filename);
					noecho();
					attroff(COLOR_PAIR(1));
					refresh();
					if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1){
						perror("Save Error: ");
						return 0;
					}
					newfl = 0;
					loadwin(start, 0);
					move(y, x);

				}
				else{
					fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
				}
				bufSave(fd, head);
				break;









			default:

					// bf->curloc = x;
					// lineInsert(bf, bf->curloc, ch);
					// loadwin(start, 0);
					// x++;

					// break;
			
				if(x >= 0 && x < LINEMAX - 1 && bf->num_chars < LINEMAX){
					bf->curloc = x;



					/*treat tab as 4 spaces*/
					if(x < LINEMAX - 4 && ch == '\t'){
						lineInsert(bf, bf->curloc, ' ');
						lineInsert(bf, bf->curloc, ' ');
						lineInsert(bf, bf->curloc, ' ');	
						lineInsert(bf, bf->curloc, ' ');
					}
					else{
						/*treat tab as 1 space*/
						if(ch == '\t')
							ch = ' ';

						lineInsert(bf, bf->curloc, ch);			


					}
					loadwin(start, 0);
					if(bf->num_chars == LINEMAX - 1 ){

					}
					else {
						if(x < LINEMAX - 4 && ch == '\t')
							x = x + 4;
						else
							x++;
					}
					
				}


				else {

				}
				break;


		
		}
    // }

		attron(COLOR_PAIR(1));
		move(ht -1, 0);
		clrtoeol();
		/*mvprintw(ht - 1, 0, "%s | row : %3d | col: %3d", filename, y, x);*/
		mvprintw(ht - 1, 0, "| filename: %s | row : %3d | col: %3d | copy: \"%s\" |", filename, bf->cur_line, x, copybuf );
		move(y, x);
		attroff(COLOR_PAIR(1));
		refresh();



        }
		

    endwin();
	close(fd);
	bufDestroy(head);
	return 0;    
}



// int main(){
// 	buffer*bf;
// 	bf = (buffer*)malloc(sizeof(buffer));
// 	bufInit(bf);
// 	printf("%d", bf->cur_line);
// 	int fd, fd2;
	
// 	fd = open("file.txt",O_RDWR);
// 	fd2 = open("file2.txt", O_RDWR);

	
// 	// bufInsert(bf);
	
// 	bufPrintAll(bf);	
// 	return 0;
// }