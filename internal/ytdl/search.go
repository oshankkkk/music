package ytdl

import (
	"encoding/json"
	"music/internal/library"
	"os/exec"
	"strings"
)

func SearchYT(query string) (library.Song,string, error) {
	metadata := exec.Command(
		"yt-dlp",
		"--dump-json",
		"ytsearch1:"+query,
	)
	audio := exec.Command(
		"yt-dlp",
		"-f", "bestaudio",
		"-g",
		"--no-playlist",
		"ytsearch1:"+query,
	)

	metaout, err := metadata.Output()

	if err != nil {
		return library.Song{}, "",err
	}

	audioout, err := audio.Output()
	audionew:=strings.TrimSpace(string(audioout))
	if err != nil {
		return library.Song{}, "",err
	}

	
	var song library.Song

	err = json.Unmarshal(metaout, &song)


	if err != nil {
		return library.Song{},"",err
	}

	return song,audionew, nil

}

//yt-dlp --dump-json "ytsearch1:daft punk harder better faster stronger"


//type response struct {
//	ID          string  `json:"id"`
//	Title       string  `json:"title"`
//	Artist      string  `json:"channel"`
//	Duration    float64 `json:"duration"`
//	Thumbnail   string  `json:"thumbnail"`
//	ViewCount   int64   `json:"view_count"`
//	UploadDate  string  `json:"upload_date"`
//	URL         string  `json:"url"`
//}



