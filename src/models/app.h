#ifndef APP_H
#define APP_H

#include <sqlite3.h>
#include "./song.h"

typedef struct{
	sqlite3 *db;
	sqlite3 *cache;
	Song *currentsong;
}App;

#endif

