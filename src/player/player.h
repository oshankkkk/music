#ifndef PLAYER_H
#define PLAYER_H

#include "../models/app.h"

int getSong(App *app,char *songName);
char *getAudioPath(App *app);
int playSong(App *app,char *songName);

#endif
