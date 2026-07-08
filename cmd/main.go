package main

import (
	"bufio"
	"fmt"
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
	if err != nil {
		fmt.Println("dataerr", err)
		os.Exit(1)
	}
	defer database.Close()

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
			
		library.AddSong(database, song)
		library.Like(database, song.ID)

			//fmt.Printf("Playing %s by %s \n", )
			library.GetSong(database,song.ID)
		fmt.Printf(" Song duration %f  \n and view count is  %d \n and the upload date is %s \n",
			song.Duration, song.ViewCount, song.UploadDate)

		if _, exists := catalogue[song.Title]; !exists {
			catalogue[song.Title] = &song
		}

		wg.Add(1)
		go func(){ if err := player.Plaympv(audio); err != nil {
			fmt.Println("Playback error:", err)

		}
			wg.Done()
	}()

	filepath,err:=ytdl.DownloadAudio(song.URL,"../audios/")
		fmt.Println(filepath)
		if err!=nil{
		fmt.Println(err)
		}
		wg.Wait()	
	}
}

