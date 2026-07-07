package player
import (
	"os/exec"
	"os"
)

func Plaympv(url string) error {
	cmd := exec.Command(
		"mpv",
		"--no-video",
		//"--loop-file=inf",
    "--input-ipc-server=/tmp/mpvsocket",
		url,
	)

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	return cmd.Run()
}


