#ifndef SONG_H
#define SONG_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef int64_t i64;

typedef struct {
    char *id;
    char *title;
    char *artist;
    double duration;
    char *thumbnail;
    i64 view_count;
    char *uploaddate;
    char *url;
    bool isliked;
    int personalplaycount;
    char *genre;
	time_t playedTime;
} Song;

#endif 
