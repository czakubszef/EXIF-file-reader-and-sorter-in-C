#include"functionsAndLibraries.h"
#include"functionsAndLibraries.c"
int main(){
    //defining variables
    char *folder[100];
    for(int i=0; i<100; i++){
        folder[i]=alloca(sizeof(char)*100);
    }
    char userInput[100];
    char destination[5000];
    char c;
    //opening ncurses window
    initscr();
    refresh();
    //asking user for folder path which contain photos
    printw("EXIF file reader\n\n");
    printw("Please enter the folder path where the photos are located:\n");
    scanw("%s", destination);
    int folderSize=openFolder(destination, folder);
    while(1){
        clear();
        printw("EXIF file reader\n\nType numer of action that you want to perform\n\n");
        printw("1. Open photo\n2. Check photo EXIF data\n3. Sort photos in selected folder");
        printw("\n\n\n\n\n\n\n\n                Press q to exit program");
        c=getch();
        switch(c){
            case'1':{
                clear();
                printw("EXIF file reader\n\n");
                for(int i=0; i<folderSize; i++){
                    printw("%s\n", folder[i]);
                }
                printw("\n\n");
                printw("Please enter photo that you want to open:\n");
                scanw("%s", userInput);
                showPhoto(userInput,destination);
                break;
            }
            case'2':{
                clear();
                printw("EXIF file reader\n\n");
                for(int i=0; i<folderSize; i++){
                    printw("%s\n", folder[i]);
                }
                printw("\n\n");
                printw("Please enter photo name that you want to check:\n");
                scanw("%s", userInput);
                checkExifData(destination,userInput);
                break;
            }
            case'3':{
                sortByExifData(destination, folder, folderSize);
                break;
            }
            case'q':{
                endwin();
                return 0;
            }
        }
    }
}