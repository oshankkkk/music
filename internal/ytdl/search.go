package ytdl

import (
	"encoding/json"
	"fmt"
	"music/internal/model"
	"os/exec"
	"strings"
)

func SearchYT(query string) (model.Song,string, error) {
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
		return model.Song{}, "",err
	}

	audioout, err := audio.Output()
	audionew:=strings.TrimSpace(string(audioout))
	if err != nil {
		return model.Song{}, "",err
	}

	var song model.Song

	err = json.Unmarshal(metaout, &song)

	if err != nil {
		return model.Song{},"",err
	}
	song.ID=generateID(song)

	return song,audionew, nil

}
func generateID(song model.Song) string{
return fmt.Sprint(song.Title,song.Artist)
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



