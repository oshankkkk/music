#ifndef YT_H
#define YT_H

#include <stdio.h>
#include "../models/response.h"

char * readAll(FILE *fp);
int ytSearch(char songName[2048], Respones *response);

#endif // YT_H
