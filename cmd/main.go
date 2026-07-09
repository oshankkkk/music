package main

import (
	"bufio"
	"fmt"
	"time"
	"music/internal/db"
	"music/internal/library"
	"music/internal/model"
	"music/internal/player"
	"music/internal/ytdl"
	"os"
	"strings"
	"sync"
)

func main() {
	fmt.Println("music player")
	fmt.Println("Type a song name:")
	var wg sync.WaitGroup
	reader := bufio.NewReader(os.Stdin)

	database, err := db.InitDB("internal/db/migrations/music.db")
	cachedb, err := db.InitDB("internal/db/migrations/cache.db")
	if err != nil {
		fmt.Println("dataerr", err)
		os.Exit(1)
	}
	defer database.Close()

//	var yt ytdl.Yt
	catalogue := make(map[string]*model.Song)
	for {
		fmt.Print("> ")
		query, _ := reader.ReadString('\n')
		query = strings.TrimSpace(query)
		if query == "exit" {
			break
		}

		fmt.Println("Searching...")
		song, audio, err := ytdl.SearchYT(query)
		if err != nil {
			fmt.Println("Search error:", err)
			continue
		}
		if !library.CheckSong(database,song.ID){
			library.AddSong(database, song)
			library.Like(database, song.ID)
		}

		//fmt.Printf("Playing %s by %s \n", )
		library.GetSong(database,song.ID)
		fmt.Printf(" Song duration %f  \n and view count is  %d \n and the upload date is %s \n",
		song.Duration, song.ViewCount, song.UploadDate)

		if _, exists := catalogue[song.Title]; !exists {
			catalogue[song.Title] = &song
		}

		if !db.CheckCacheAudio(cachedb,song.ID){
			if err := player.Plaympv(audio); err != nil {
				fmt.Println("Playback error:", err)

			}

			wg.Add(1)
			go func() {
				filepath, err := ytdl.DownloadAudio(song.URL, fmt.Sprintf("../cache/%s",song.ID))
				if err != nil {
					fmt.Println(err)
					return
				}

				expireTime := time.Now().Add(24 * time.Hour)

				db.AddToCache(cachedb, song.ID, filepath, expireTime)

				fmt.Println(filepath)

				wg.Done()
			}()
		}else{
			filepath:=db.GetCacheAudio(cachedb,song.ID)
			if err := player.Plaympv(filepath); err != nil {
				fmt.Println("Playback error:", err)

			}

		}

		wg.Wait()	
	}
}

