package ytdl
import (
	"os/exec"
	"strings"	
)
func SearchYT(query string) (string, error) {
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
	return strings.TrimSpace(string(out)), nil
}


