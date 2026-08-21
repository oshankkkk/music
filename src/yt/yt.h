#ifndef YT_H
#define YT_H

#include <stdio.h>
#include "../models/song.h"
#include "../models/app.h"

char * readAll(FILE *fp);
int ytSearch(char *songName, Song *app);
char *ytDownload(char *filepath, char *url);
#endif // YT_H
