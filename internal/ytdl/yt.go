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
	metaout, err := metadata.Output()
	if err != nil {
		return model.Song{}, "",err
	}
	//parts := strings.SplitN(strings.TrimSpace(string(metaout)), "\x1f", 2)
	var song model.Song

	err = json.Unmarshal(metaout, &song)

	if err != nil {
		return model.Song{},"",err
	}

	audio := exec.Command(
		"yt-dlp",
		"-f", "bestaudio",
		"-g",
		"--no-playlist",
		song.URL,
		//"https://www.youtube.com/watch?v="+parts[0],
	)

	audioout, err := audio.Output()
	audionew:=strings.TrimSpace(string(audioout))
	if err != nil {
		return model.Song{}, "",err
	}

	song.ID=generateID(song)

	return song,audionew, nil
}
 
func DownloadAudio(url, destPathNoExt string) (string, error) {
	cmd := exec.Command("yt-dlp",
		"-f", "bestaudio",
		"-x", "--audio-format", "mp3",
		"-o", destPathNoExt+".%(ext)s",
		url,
		//"https://www.youtube.com/watch?v="+videoID,
	)
	if err := cmd.Run(); err != nil {
		return "", err
	}
	fmt.Println("download una")
	return destPathNoExt + ".mp3", nil
}

func generateID(song model.Song) string{
return fmt.Sprint(song.Title,song.Artist)
}



