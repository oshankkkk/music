package db

import(
	"database/sql"
	"time"
	"fmt"
	"music/internal/db/cache"
)

func AddToCache(store *sql.DB, id string, filepath string, expiresAt time.Time) {
	err:=cache.AddToCache(store,id,filepath,expiresAt)
	if err!=nil{
		fmt.Println(err)
	}

}

func GetCacheAudio(store*sql.DB, id string)string {
	filepath,err:=cache.GetCacheAudio(store,id)
	if err!=nil{
		fmt.Println("getaudio",err)
	}
	return filepath
}

func CheckCacheAudio(store*sql.DB, id string) bool {
	return cache.CheckCacheAudio(store,id)
}

