#include <stdbool.h>
#include <stdint.h>

typedef int64_t i64 ;

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
} Song;


