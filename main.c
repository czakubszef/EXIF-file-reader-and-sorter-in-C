#include<stdlib.h>
#include<stdio.h>
#include<ncurses.h>
#include<string.h>
#include<alloca.h>
#include<dirent.h>
#include<libexif/exif-data.h>
#include<libexif/exif-log.h>
#include<libexif/exif-mem.h>
#include<libexif/exif-loader.h>
#include<libexif/exif-tag.h>
#include<libexif/exif-entry.h>
//Function that allow user to open folder where he want to do something with photos. It returns pointer to file with list of all files that are in folder pointed by user
FILE *openFolder(char* destination){
    char systemInput[200];
    const char *systemOutput="systemOutput.txt";
    sprintf(systemInput,"ls %s > %s", destination, systemOutput);
    FILE *file=fopen(systemOutput,"r");
    return file;
}
//Function that allows to show photo
void showPhoto(char* userInput, char* destination){
    char *systemInput;
    systemInput=alloca(sizeof(*systemInput)*(strlen(userInput)+strlen(destination)+strlen("open ")));
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
    return;
}
//Function that checks and print on screen EXIF data contained in JPEG photo
void checkExifData(char *destination, char *userInput){
    char *exifInput=alloca(sizeof(*exifInput)*(strlen(destination)+strlen(userInput)));
    sprintf(exifInput,"%s/%s", destination, userInput);
    ExifData *photo=exif_data_new_from_file(exifInput);
    const char *messages[]={"Date and time when photo was taken:", "Model of camera:", "Expousure time:", "Aperature:", "ISO:"};
    int tags[]={EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_TAG_MODEL, EXIF_TAG_EXPOSURE_TIME, EXIF_TAG_MAX_APERTURE_VALUE, EXIF_TAG_ISO_SPEED_RATINGS};
    if(!photo){
        printw("Can't open this file.\nPress anything to return to menu.");
        getch();
        return;
    }
    for(int i=0; i<5; i++){
        printExifData(photo, tags[i], messages[i]);
    }
    printw("Press anything to return to menu.");
    getch();
    free(userInput);
}
void showSortedPhotos(char* userOption, char *destination, int tag){
    clear();
    printw("EXIF file reader\n\n");
    printw("Here are photos with same selected value of EXIF tag:\n\n");
    char c;
    const char *systemOutput="systemOutput.txt";
    FILE *inputFile=fopen(systemOutput,"r");
    char fileName[200];
    if(inputFile){
        for(int i=0; (c=getc(inputFile))!=EOF; i++){
            if(c=='\n'){
                char *exifInput=alloca(sizeof(char)*200);
                sprintf(exifInput,"%s/%s", destination, fileName);
                ExifData *photo=exif_data_new_from_file(exifInput);
                ExifEntry *data=exif_data_get_entry(photo,tag);
                char dataOut[1000];
                exif_entry_get_value(data, dataOut, sizeof(dataOut));
                if(strcmp(dataOut,userOption)==0){
                    printw("%s\n", fileName);
                    
                }
                for(int j=0; j<=strlen(fileName); j++){
                    fileName[j]='\0';
                }
                i=0;
                continue;
            }
            fileName[i]=c;
        }
    }
    printw("\nWhat do you want to do?\n");
    printw("1. Show one of these photos\n");
    printw("2. Check EXIF data of one of these photos\n");
    printw("3. Press q to return to menu\n\n");
    c=getch();
    switch(c){
        case'1':{
            printw("Type name of photo:\n");
            char userInput[100];
            scanw("%s", userInput);
            showPhoto(userInput, destination);
            break;
        }
        case'2':{
            printw("Type name of photo:\n");
            char userInput[100];
            scanw("%s", userInput);
            checkExifData(destination,userInput);
            break;
        }
        case'q':{
            fclose(inputFile);
            system("rm -r sortedPhotos.txt");
            break;
        }
    }
}
void sortOptions(int tag, char *destination){
    clear();
    printw("EXIF file reader\n\n");
    char *sortList[200];
    for(int i=0; i<200; i++){
        sortList[i]=alloca(sizeof(char)*300);
        sortList[i]="\0";
    }
    char *fileName=alloca(sizeof(char)*100);
    char c;
    int sortListNum=0;
    struct dirent *file;
    DIR *path;
    path=opendir(destination);
    while(file=readdir(path)){
        char *exifInput=alloca(sizeof(char)*1000);
        fileName=file->d_name;
        sprintf(exifInput,"%s/%s", destination, fileName);
        ExifData *photo=exif_data_new_from_file(exifInput);
        ExifEntry *data=exif_data_get_entry(photo,tag);
        char dataOut[1000];
        exif_entry_get_value(data, dataOut, sizeof(dataOut));
        int z=0;
        for(int i=0; i<sortListNum; i++){
            if(strcmp(dataOut, sortList[i])==0){
                z=1;
                break;
            }
        }
        if(z==0){
            sprintf(sortList[sortListNum], "%s", dataOut);
            sortListNum++;
        }
    }
    printw("What data you want to see photos with:\n");
    for(int i=0; i<sortListNum; i++){
        printw("%d. %s\n", i, sortList[i]);
    }
    c=getch();
    int n=c-'0';
    char* userOption=sortList[n];
    showSortedPhotos(userOption,destination,tag);
}
//function that allow user to sort their photos by making another folder which contain picked photos 
void sortByExifData(char *destination){
    clear();
    printw("EXIF file reader\n\n");
    printw("Choose how you want to sort:\n\n");
    printw("1. By date and time\n2. By camera model\n3. By expusure time\n4. By aperature\n5. By ISO\n\n\n\n\n           Press q to return to menu\n");
    int tags[]={EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_TAG_MODEL, EXIF_TAG_EXPOSURE_TIME, EXIF_TAG_MAX_APERTURE_VALUE, EXIF_TAG_ISO_SPEED_RATINGS};
    char c;
    c=getch();
    switch(c){
        case'q':{
            return;
        }
        case'1':{
            sortOptions(tags[0], destination);
            break;
        }
        case'2':{
            sortOptions(tags[1], destination);
            break;
        }
        case'3':{
            sortOptions(tags[2], destination);
            break;
        }
        case'4':{
            sortOptions(tags[3], destination);
            break;
        }
        case'5':{
            sortOptions(tags[4], destination);
            break;
        }
    }
}
int main(){
    //defining variables
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
                FILE *file=openFolder(destination);
                if(file){
                    while((c=getc(file))!=EOF){
                        printw("%c", c);
                    }
                }
                printw("\n\n");
                printw("Please enter photo that you want to open:\n");
                scanw("%s", userInput);
                showPhoto(userInput,destination);
                fclose(file);
                break;
            }
            case'2':{
                clear();
                printw("EXIF file reader\n\n");
                FILE *file=openFolder(destination);
                if(file){
                    while((c=getc(file))!=EOF){
                        printw("%c", c);
                    }
                }
                printw("\n\n");
                printw("Please enter photo name that you want to check:\n");
                scanw("%s", userInput);
                checkExifData(destination,userInput);
                fclose(file);
                break;
            }
            case'3':{
                sortByExifData(destination);
                break;
            }
            case'q':{
                endwin();
                break;
            }
        }
    }
}