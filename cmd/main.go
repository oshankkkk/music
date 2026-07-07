package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"music/internal/player"
	"music/internal/ytdl"
)

func main() {
	reader := bufio.NewReader(os.Stdin)

	fmt.Println("music player")
	fmt.Println("Type a song name:")

	for {
		fmt.Print("> ")
		query, _ := reader.ReadString('\n')
		query = strings.TrimSpace(query)

		if query == "exit" {
			break
		}

		fmt.Println("Searching...")

		song,audio, err := ytdl.SearchYT(query)
		if err != nil {
			fmt.Println("Search error:", err)
			continue
		}

		//fmt.Println("Playing:", audio)
		fmt.Printf("Playing %s by %s \n", song.Title,song.Artist)
		fmt.Printf(" Song duration %f  \n and view count is  %d \n and the upload data is %s \n", song.Duration,song.ViewCount,song.UploadDate)

		err = player.Plaympv(audio)
		if err != nil {
			fmt.Println("Playback error:", err)
		}
	}
}
