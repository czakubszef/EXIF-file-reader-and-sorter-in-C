#include"functionsAndLibraries.h"
#include <assert.h>

int openFolder(char* destination, char*folder[]){
    struct dirent *file;
    int folderSize=0;
    DIR *path;
    path=opendir(destination);
    char *fileName;
    while((file=readdir(path))!=NULL){
        fileName=file->d_name;
        if(strcmp(fileName,".")==0 || strcmp(fileName,"..")==0 || strcmp(fileName,"")==0){
           continue;
        }
        strcpy(folder[folderSize], fileName);
        folderSize++;
    }
    return folderSize;
}
//Function that allows to show photo
void showPhoto(char* userInput, char* destination){
    char *systemInput;
    systemInput=alloca(sizeof(*systemInput)*(strlen(userInput)+strlen(destination)+strlen("open ")+2));
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
    char *exifInput=alloca(sizeof(*exifInput)*(strlen(destination)+strlen(userInput)+2));
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
}
void showSortedPhotos(char* userOption, char *destination, int tag, char *folder[], int folderSize){
    clear();
    printw("EXIF file reader\n\n");
    printw("Here are photos with same selected value of EXIF tag:\n\n");
    char c;
    char *fileName;
    for(int i=0; i<folderSize; i++){
        fileName = folder[i];
        char *exifInput=alloca(sizeof(char)*200);
        sprintf(exifInput,"%s/%s", destination, fileName);
        ExifData *photo=exif_data_new_from_file(exifInput);
        ExifEntry *data=exif_data_get_entry(photo,tag);
        char dataOut[1000];
        exif_entry_get_value(data, dataOut, sizeof(dataOut));
        if(strcmp(dataOut,userOption)==0){
            printw("%s\n", fileName);
            
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
            break;
        }
    }
}
void sortOptions(int tag, char *destination, char *folder[], int folderSize){
    clear();
    printw("EXIF file reader\n\n");
    char *sortList[200];
    for(int i=0; i<200; i++){
        sortList[i]=alloca(sizeof(char)*300);
        sortList[i][0]='\0';
    }
    char *fileName;
    int sortListNum=0;
    char *exifInput=alloca(sizeof(char)*1000);
    for(int i=0; i<folderSize; i++){
        fileName = folder[i];
        printf("\n%s", fileName);
        sprintf(exifInput,"%s/%s", destination, fileName);
        ExifData *photo=exif_data_new_from_file(exifInput);
        if(photo==NULL){
            continue;
        }
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
    char c=getch();
    int n=c-'0';
    char* userOption=sortList[n];
    showSortedPhotos(userOption,destination,tag, folder, folderSize);
}
//function that allow user to sort their photos by making another folder which contain picked photos 
void sortByExifData(char *destination, char* folder[], int folderSize){
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
            sortOptions(tags[0], destination, folder, folderSize);
            break;
        }
        case'2':{
            sortOptions(tags[1], destination, folder, folderSize);
            break;
        }
        case'3':{
            sortOptions(tags[2], destination, folder, folderSize);
            break;
        }
        case'4':{
            sortOptions(tags[3], destination, folder, folderSize);
            break;
        }
        case'5':{
            sortOptions(tags[4], destination, folder, folderSize);
            break;
        }
    }
}