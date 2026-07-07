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

		url, err := ytdl.SearchYT(query)
		if err != nil {
			fmt.Println("Search error:", err)
			continue
		}

		fmt.Println("Playing:", url)

		err = player.Plaympv(url)
		if err != nil {
			fmt.Println("Playback error:", err)
		}
	}
}
