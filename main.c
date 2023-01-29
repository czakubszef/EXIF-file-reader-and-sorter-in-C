#include<stdlib.h>
#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<libexif/exif-data.h>
#include<libexif/exif-log.h>
#include<libexif/exif-mem.h>
#include<libexif/exif-loader.h>
#include<libexif/exif-tag.h>
#include<libexif/exif-entry.h>
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
void checkExifData(char *destination, char *userInput){
    char *exifInput=malloc(sizeof(char)*150);
    strcat(exifInput, destination);
    strcat(exifInput, userInput);
    ExifData *photo=exif_data_new_from_file(exifInput);
    if(!photo){
        printw("Can't open this file.\nPress anything to return to menu.");
        char c=getch();
        if(c){
            free(userInput);
            return;
        }
    }
    ExifEntry *data=exif_data_get_entry(photo,EXIF_TAG_DATE_TIME_ORIGINAL);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("Date and time when photo was taken: %s\n", dataOutput);
    }
    data=exif_data_get_entry(photo,EXIF_TAG_MODEL);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("Model of camera: %s\n", dataOutput);
    }
    data=exif_data_get_entry(photo,EXIF_TAG_LENS_MODEL);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("Model of lens: %s\n", dataOutput);
    }
    data=exif_data_get_entry(photo,EXIF_TAG_EXPOSURE_TIME);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("Expousure time: %s\n", dataOutput);
    }
    data=exif_data_get_entry(photo,EXIF_TAG_MAX_APERTURE_VALUE);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("Aperature: %s\n", dataOutput);
    }
    data=exif_data_get_entry(photo,EXIF_TAG_ISO_SPEED_RATINGS);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("ISO: %s\n", dataOutput);
    }
    printw("Press anything to return to menu.");
    char c=getch();
    if(c){
        free(userInput);
        return;
    }
}
int main(){
    //defining variables
    char* userInput;
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
        printw("1. Open photo\n2. Check photo EXIF data");
        printw("\n\n\n\n\n\n\n\n                Press q to exit programm");
        c=getch();
        if(c=='1'){
            clear();
            printw("EXIF file reader\n\n");
            if(file){
                while((c=getc(file))!=EOF){
                    printw("%c", c);
                }
            }
            printw("\n\n");
            printw("Please enter photo that you want to open:\n");
            userInput=malloc(sizeof(char)*100);
            scanw("%s", userInput);
            showPhoto(userInput,destination);
        }
        if(c=='2'){
            clear();
            printw("EXIF file reader\n\n");
            if(file){
                while((c=getc(file))!=EOF){
                    printw("%c", c);
                }
            }
            printw("\n\n");
            printw("Please enter photo name that you want to check:\n");
            userInput=malloc(sizeof(char)*100);
            scanw("%s", userInput);
            checkExifData(destination,userInput);
        }
        else if(c=='q'){
            free(destination);
            free(userInput);
            endwin();
        }
    }
}