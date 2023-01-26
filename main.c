#include<stdlib.h>
#include<stdio.h>
#include<ncurses.h>
#include<string.h>
//#include"DevIL/include/IL/il.h"
//Function that allow user to open folder where he want to do something with photos. It returns pointer to file with list of all files that are in folder pointed by user
FILE *openFolder(char* destination){
    char c;
    char *systemInput=malloc(sizeof(char)*123);
    strcat(systemInput,"ls ");
    strcat(systemInput,destination);
    strcat(systemInput," > systemOutput.txt");
    system(systemInput);
    FILE *file=fopen("systemOutput.txt","r");
    free(systemInput);
    return file;
}
//Function that allows to show photo
void showPhoto(char* userInput, char* destination){
    char *systemInput=malloc(sizeof(char)*150);
    strcat(systemInput,"open ");
    strcat(systemInput,destination);
    strcat(systemInput,userInput);
    system(systemInput);
    free(systemInput);
    free(userInput);
    return;
}
int main(){
    //defining variables
    char* userInput=malloc(sizeof(char)*100);
    char* destination=malloc(sizeof(char)*100);
    char c;
    //opening ncurses window
    initscr();
    refresh();
    //asking user for folder path which contain photos
    printw("EXIF file reader\n\n");
    printw("Please enter the folder path where the photos are located:\n");
    scanw("%s", destination);
    FILE *file=openFolder(destination);
    while(1){
        clear();
        printw("EXIF file reader\n\nType numer of action that you want to perform\n\n");
        printw("1. Open photo\n");
        printw("\n\n\n\n\n\n\n\n                Press q to exit programm");
        c=getch();
        if(c=='1'){
            clear();
            userInput=malloc(sizeof(char)*100);
            printw("EXIF file reader\n\n");
            if(file){
                while((c=getc(file))!=EOF){
                    printw("%c", c);
                }
            }
            printw("\n\n");
            printw("Please enter photo that you want to open:\n");
            scanw("%s", userInput);
            showPhoto(userInput,destination);
        }
        else if(c=='q'){
            free(destination);
            free(userInput);
            endwin();
        }
    }
}