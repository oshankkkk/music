#ifndef YT_H
#define YT_H

#include <stdio.h>
#include "../models/song.h"

char * readAll(FILE *fp);
int ytSearch(char songName[2048], Song *song);
char *ytDownload(char *filepath, char *url);
#endif // YT_H
