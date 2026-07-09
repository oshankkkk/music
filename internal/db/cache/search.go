package cache
//import (
//	"bufio"
//	"database/sql"
//	"fmt"
//	"log"
//	"os"
//	"os/exec"
//	"strings"
//	"time"
//
//	_ "github.com/mattn/go-sqlite3"
//)
//
//const (
//	dbPath = "cache.db"
//	// googlevideo stream URLs are signed and expire (~6h observed).
//	// Refresh a bit before that so we don't hand mpv a dead link.
//	urlTTL = 5 * time.Hour
//)
//
//type track struct {
//	videoID string
//	title   string
//	url     string
//}
//
////func initDB() (*sql.DB, error) {
////	db, err := sql.Open("sqlite", dbPath)
////	if err != nil {
////		return nil, err
////	}
////	_, err = db.Exec(`
////		CREATE TABLE IF NOT EXISTS tracks (
////			query       TEXT PRIMARY KEY,
////			video_id    TEXT NOT NULL,
////			title       TEXT NOT NULL,
////			url         TEXT NOT NULL,
////			resolved_at INTEGER NOT NULL
////		);
////	`)
////	return db, err
////}
//
//// searchMeta resolves a query to a stable video id + title. This never
//// expires, so it's the thing we cache permanently keyed by query text.
//func searchMeta(query string) (id, title string, err error) {
//	cmd := exec.Command(
//		"yt-dlp",
//		"--print", "%(id)s\x1f%(title)s",
//		"--skip-download",
//		"ytsearch1:"+query,
//	)
//	out, err := cmd.Output()
//	if err != nil {
//		return "", "", err
//	}
//
//	parts := strings.SplitN(strings.TrimSpace(string(out)), "\x1f", 2)
//	if len(parts) != 2 {
//		return "", "", fmt.Errorf("unexpected yt-dlp metadata output: %q", out)
//	}
//	return parts[0], parts[1], nil
//}
//
//// resolveURL fetches a fresh signed stream URL for a known video id.
//// Cheaper than a full search since it skips ytsearch entirely.
//func resolveURL(videoID string) (string, error) {
//	cmd := exec.Command(
//		"yt-dlp",
//		"-f", "bestaudio",
//		"-g",
//		"https://www.youtube.com/watch?v="+videoID,
//	)
//	out, err := cmd.Output()
//	if err != nil {
//		return "", err
//	}
//	return strings.TrimSpace(string(out)), nil
//}
//
//// getTrack is the cache-through lookup: miss -> full search, stale -> re-resolve
//// url only, fresh hit -> zero subprocess calls.
//func getTrack(db *sql.DB, query string) (track, error) {
//	var t track
//	var resolvedAt int64
//
//	err := db.QueryRow(
//		`SELECT video_id, title, url, resolved_at FROM tracks WHERE query = ?`,
//		query,
//	).Scan(&t.videoID, &t.title, &t.url, &resolvedAt)
//
//	switch {
//	case err == sql.ErrNoRows:
//		fmt.Println("Searching...")
//		id, title, err := searchMeta(query)
//		if err != nil {
//			return track{}, fmt.Errorf("search: %w", err)
//		}
//		url, err := resolveURL(id)
//		if err != nil {
//			return track{}, fmt.Errorf("resolve url: %w", err)
//		}
//		t = track{videoID: id, title: title, url: url}
//		_, err = db.Exec(
//			`INSERT INTO tracks (query, video_id, title, url, resolved_at) VALUES (?, ?, ?, ?, ?)`,
//			query, t.videoID, t.title, t.url, time.Now().Unix(),
//		)
//		return t, err
//
//	case err != nil:
//		return track{}, err
//
//	case time.Since(time.Unix(resolvedAt, 0)) > urlTTL:
//		fmt.Println("Cached link expired, refreshing stream URL...")
//		url, err := resolveURL(t.videoID)
//		if err != nil {
//			return track{}, fmt.Errorf("resolve url: %w", err)
//		}
//		t.url = url
//		_, err = db.Exec(
//			`UPDATE tracks SET url = ?, resolved_at = ? WHERE query = ?`,
//			url, time.Now().Unix(), query,
//		)
//		return t, err
//
//	default:
//		fmt.Println("Cache hit ⚡")
//		return t, nil
//	}
//}
//
//func playWithMPV(url string) error {
//	cmd := exec.Command("mpv", "--no-video", url)
//	cmd.Stdout = os.Stdout
//	cmd.Stderr = os.Stderr
//	return cmd.Run()
//}
//
//func main() {
//	db, err := initDB()
//	if err != nil {
//		log.Fatal("db init:", err)
//	}
//	defer db.Close()
//
//	reader := bufio.NewReader(os.Stdin)
//	fmt.Println("Go Music Prototype 🎵")
//	fmt.Println("Type a song name:")
//	for {
//		fmt.Print("> ")
//		query, _ := reader.ReadString('\n')
//		query = strings.TrimSpace(query)
//		if query == "exit" {
//			break
//		}
//		if query == "" {
//			continue
//		}
//
//		t, err := getTrack(db, query)
//		if err != nil {
//			fmt.Println("Search error:", err)
//			continue
//		}
//
//		fmt.Println("Playing:", t.title)
//		if err := playWithMPV(t.url); err != nil {
//			fmt.Println("Playback error:", err)
//		}
//	}
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////
////import (
////	"database/sql"
////	//"errors"
////	"fmt"
////
////	"music/internal/model"
////	_ "github.com/mattn/go-sqlite3"
////)
////func CacheData(db *sql.DB, song model.Song){ 
////	_, err := db.Exec(`
////
////	INSERT OR REPLACE INTO cache
////	(id, value, created_at, expires_at)
////	VALUES (?, ?, ?, ?)
////	`,
////
////	id,
////	data,
////	time.Now().Unix(),
////	time.Now().Add(10*time.Minute).Unix(),
////)
////	if err != nil {
////		fmt.Println(err)
////	}
////
////}ackage cache
