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
int openFolder(char* destination, char*folder[]);
void showPhoto(char* userInput, char* destination);
void printExifData(ExifData *photo, int exifTag, const char *msg);
void checkExifData(char *destination, char *userInput);
void showSortedPhotos(char* userOption, char *destination, int tag, char *folder[], int folderSize);
void sortOptions(int tag, char *destination, char *folder[], int folderSize);
void sortByExifData(char *destination, char* folder[], int folderSize);