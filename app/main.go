package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
	"strings"
)

func searchYT(query string) (string, error) {
	cmd := exec.Command(
		"yt-dlp",
		"-f", "bestaudio",
		"-g",
		"ytsearch1:"+query,
	)

	out, err := cmd.Output()
	if err != nil {
		return "", err
	}
	fmt.Println(out,"this is the url")
	return strings.TrimSpace(string(out)), nil
}

func plaympv(url string) error {
	cmd := exec.Command(
		"mpv",
		"--no-video",
		url,
	)

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	return cmd.Run()
}

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

		url, err := searchYT(query)
		if err != nil {
			fmt.Println("Search error:", err)
			continue
		}

		fmt.Println("Playing:", url)

		err = plaympv(url)
		if err != nil {
			fmt.Println("Playback error:", err)
		}
	}
}
