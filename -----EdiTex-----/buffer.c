#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "buffer.h"
#include "UI.h"


/*initialize line*/
//This is to mallocate The size of line of Character


void lineInit(buffer *bf){
	bf->line = (char *)malloc(sizeof(char) * LINEMAX);
	bf->line = memset(bf->line, '\0', LINEMAX);

}




//Initialize Buffer :
void bufInit(buffer *bf){
    bf->next = NULL;
    bf->prev = NULL;
    bf ->cur_line =0;
    bf ->num_chars =0;
    lineInit(bf);

}


//Now if  Next If Next Buffer is NULL 
// We have to Create 


void bufCreateNext(buffer *bf){
    bf -> next = (buffer *) malloc (sizeof(buffer)*1);
    bf->next->prev =bf;
    bf->next->next = NULL;
    //next line
    bf->next->cur_line= bf->cur_line +1;

    bf = bf->next;

    bf->num_chars = 0;
    lineInit(bf);
}

//To Increase the Line number
void bufIncr(buffer *bf, int val){
	while(bf->next != NULL){
		bf->cur_line = bf->cur_line + val;
		bf = bf->next;
	}
	(bf->cur_line)++;
}

//To Deacrease the Line Number

void bufDecr(buffer *bf, int val){
    while(bf->next != NULL){
        bf->cur_line = bf->cur_line - val;
        bf = bf->next;
    }
    (bf->cur_line)--;
}

//Insert a BUFERR
//between the current and Next

void bufInsert(buffer *bf){
    //If bf ka next is NULL then Create New Buffer

    if(bf->next == NULL){
        bufCreateNext(bf);
        return;
    }

    bufIncr(bf->next,1);
    buffer *temp;
    temp = (buffer*)malloc (sizeof(buffer));

    temp->next = bf->next;
    temp->prev = bf;

    bf->next ->prev =temp;
    bf->next =temp;

    bf->next->cur_line=bf->cur_line+1;
    bf->next -> num_chars =0;

    lineInit(bf->next);


}

void bufDestroy(buffer*bf){
    buffer*temp;

    while(bf != NULL){
        temp = bf;
        bf = bf->next;
        free(temp->line);
        free(temp);
    }
}

// File Descrepter=fd
//this is method to save the buffer 

// matlab fd is will geive adress of open file
//lseek = will return offset adress otherwise -1;

void bufSave(int fd, buffer *bf){
    if(lseek(fd, 0, SEEK_SET) == -1){
        INFO;
        perror("lseek error: ");
    }
    while(bf != NULL){
        if(write(fd, bf->line, bf->num_chars) == -1){
            INFO;
            perror("Write error: ");
        }
        bf = bf->next;
    }
}


//adss the data into the buffers from text file 
void bufLoad(int fd, buffer *bf){
    char ch;
    int err, linenum = 0, i = 0;
    while((err = read(fd, &ch, 1))){
        if(err == -1){
            INFO;
            perror("Read Error: ");
            exit(0);
        }
        if(i < LINEMAX){

            if(ch == '\n'){
                
                bufCreateNext(bf);
                bf->line[i] = ch;
                bf->cur_line = linenum;
                bf->num_chars = i+1;
                bf = bf->next;
                linenum++;
                i = 0;
                continue;
            }
            else{

                bf->line[i] = ch;
                bf->num_chars = i;

            }

        }
        else{
            
            bufCreateNext(bf);
            bf->line[i] = ch;
            bf->cur_line = linenum;
            bf->num_chars = i+1;
            bf = bf->next;
            linenum++;
            i = 0;
            continue;

        }
    i++;
    }
    bf = bf->prev;
    free(bf->next);
    bf->next = NULL;
}

// this function will print all the contents of the Buffer

void bufPrintAll(buffer *bf){

    int i = 0;
    while(bf != NULL){
        printf("line no: %d\t num_chars: %d\t\t@", bf->cur_line, bf->num_chars);
        for(i = 0; i < bf->num_chars; i++){
            if(*(bf->line + i) == '\0'){
                printf("#");
            }	
            else{
                printf("%c", *(bf->line + i));
                
            }	
        }
        printf("@");
        i = 0;
        
        bf = bf->next;
    }
}


//2) Line Operations :

 void lineInsert(buffer *bf, int loc, char ch){

    if(loc == bf->num_chars){
        bf->line[loc] = ch;
        (bf->num_chars)++;
    }
    else if(bf->num_chars == LINEMAX){
        return;
    }
    else if(bf->num_chars == 0){
        bf->line[0] = ch;
        (bf->num_chars)++;
    }
    else if(bf->num_chars < LINEMAX && bf->num_chars > 0 && loc != LINEMAX && loc != LINEMAX - 1){

        // memmove() is used to copy a block of memory from a location to another. It is declared in string.h
        memmove((bf->line + loc + 1), (bf->line + loc), bf->num_chars - loc);
        bf->line[loc] = ch;
        (bf->num_chars)++;
    }
    return;
}

int fileexist(char const *filename){
     int fd = open(filename, O_RDWR );   /* open file in read/write mode*/
     if(fd == -1)                   /* couldn't open i.e. file not existing*/
        return 0;
     close(fd);   /*close file if opened i.e. file exists*/
     return 1;
}


//###########################Notes:
// memset() is used to fill a block of memory with a particular value.
// The syntax of memset() function is as follows :

// // ptr ==> Starting address of memory to be filled
// // x   ==> Value to be filled
// // n   ==> Number of bytes to be filled starting 
// //         from ptr to be filled
// void *memset(void *ptr, int x, size_t n);