#ifndef APP_H
#define APP_H

#include <sqlite3.h>
#include "./song.h"
#include "./msg.h"

typedef struct{
	sqlite3 *db;
	sqlite3 *cache;
	Song *currentsong;
	int mpvfd;
	int serverfd;
	int clientfd;
	queue *msgqueue;

}App;

#endif

