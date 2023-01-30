#include<stdlib.h>
#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<alloca.h>
#include<libexif/exif-data.h>
#include<libexif/exif-log.h>
#include<libexif/exif-mem.h>
#include<libexif/exif-loader.h>
#include<libexif/exif-tag.h>
#include<libexif/exif-entry.h>
//Function that allow user to open folder where he want to do something with photos. It returns pointer to file with list of all files that are in folder pointed by user
FILE *openFolder(char* destination){
    char c;
    char systemInput[200];
    const char *systemOutput="systemOutput.txt";
    sprintf(systemInput,"ls %s > %s", destination, systemOutput);
    FILE *file=fopen(systemOutput,"r");
    return file;
}
//Function that allows to show photo
void showPhoto(char* userInput, char* destination){
    char *systemInput;
    alloca(sizeof(systemInput)*(strlen(userInput)+strlen(destination)+strlen("open ")));
    sprintf(systemInput,"open %s/%s", destination, userInput);
    system(systemInput);
    return;
}
void printExifData(ExifData *photo, int exifTag, const char *msg){
    ExifEntry *data=exif_data_get_entry(photo, exifTag);
    if(data){
        char dataOutput[1000];
        exif_entry_get_value(data, dataOutput, sizeof(dataOutput));
        printw("%s %s\n", msg, dataOutput);
    }
}
//Function that checks and print on screen EXIF data contained in JPEG photo
void checkExifData(char *destination, char *userInput){
    char *exifInput;
    alloca(sizeof(exifInput)*(strlen(destination)+strlen(userInput)));
    sprintf(exifInput,"%s/%s", destination, userInput);
    ExifData *photo=exif_data_new_from_file(exifInput);
    const char *messages[]={"Date and time when photo was taken:", "Model of camera:", "Model of lens:", "Expousure time:", "Aperature:", "ISO:"};
    int tags[]={EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_TAG_MODEL, EXIF_TAG_LENS_MODEL, EXIF_TAG_EXPOSURE_TIME, EXIF_TAG_MAX_APERTURE_VALUE, EXIF_TAG_ISO_SPEED_RATINGS};
    if(!photo){
        printw("Can't open this file.\nPress anything to return to menu.");
        getch();
        return;
    }
    for(int i=0; i<6; i++){
        printExifData(photo, tags[i], messages[i]);
    }
    printw("Press anything to return to menu.");
    getch();
    free(userInput);
}
//function that allow user to sort their photos by making another folder which contain picked photos 
/*void sortByExifData(char *destination){
    clear();
    printw("EXIF file reader\n\n");
    printw("Choose how you want to sort:\n\n");
    printw("1. By date and time\n2. By camera model\n3. By expusure time\n4. By aperature\n5. By ISO\n\n\n\n\n           Press q to return to menu");
    char c=getch();
    if(c=='q'){
        return;
    }
    if(c=='1'){
        clear();
        printw("EXIF file reader\n\n");
        FILE *file=fopen("systemOutput.txt","r");
        char sortList[50][50];
        char fileName[50];
        if(file){
            int sortListNum=0;
            while((c=getc(file))!=EOF){
                int character=0;
                while(1){
                    if(c=="\n"){
                        char *exifInput=malloc(sizeof(char)*150);
                        strcat(exifInput, destination);
                        strcat(exifInput, fileName);
                        ExifData *photo=exif_data_new_from_file(exifInput);
                        ExifEntry *data=exif_data_get_entry(photo,EXIF_TAG_DATE_TIME_ORIGINAL);
                        char dataOut[1000];
                        exif_entry_get_value(data, dataOut, sizeof(dataOut));
                        int z=0;
                        for(int i=0; i<=sortListNum; i++){
                            if(strcmp(dataOut, sortList[i])==0){
                                z=1;
                                continue;
                            }
                        }
                        if(z==0){
                            sortListNum++;
                            for(int i=0; i<sizeof(dataOut); i++){
                                sortList[sortListNum][i]=dataOut[i];
                            }
                        }
                        break;
                    }
                    fileName[character]=c;
                    character++;
                }
            }
        }
    }
}*/
int main(){
    //defining variables
    char userInput[100];
    char destination[1000];
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
        switch(c){
            case'1':{
                clear();
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
                break;
            }
            case'2':{
                clear();
                printw("EXIF file reader\n\n");
                if(file){
                    while((c=getc(file))!=EOF){
                        printw("%c", c);
                    }
                }
                printw("\n\n");
                printw("Please enter photo name that you want to check:\n");
                scanw("%s", userInput);
                checkExifData(destination,userInput);
                break;
            }
            case'q':{
                endwin();
                break;
            }
        }
    }
}