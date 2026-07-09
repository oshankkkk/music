package library

import (
	"database/sql"
	"fmt"
	"music/internal/db"
	"music/internal/model"
)

func CheckSong(store *sql.DB, id string) bool {
	res, err := db.CheckSong(store, id)
	if err != nil {
		fmt.Println("CheckSong error:")
		fmt.Println(err)
	}
	return res
}

func AddSong(store *sql.DB, song model.Song) {
	err := db.AddSong(store, song)
	if err != nil {
		fmt.Println("AddSong error:")
		fmt.Println(err)
	}
}

func RemoveSong(store *sql.DB, id string) {
	err := db.RemoveSong(store, id)
	if err != nil {
		fmt.Println("RemoveSong error:")
		fmt.Println(err)
	}
}

func GetSong(store *sql.DB, id string) model.Song {
	song, err := db.GetSong(store, id)
	if err != nil {
		fmt.Println("GetSong error:")
		fmt.Println(err)
	}
	fmt.Printf("menna song %s\n", song.Title)
	return song
}

func Play(store *sql.DB, id string) {
	err := db.Play(store, id)
	if err != nil {
		fmt.Println("Play error:")
		fmt.Println(err)
	}
}

func Like(store *sql.DB, id string) {
	fmt.Println("like like")
	err := db.Like(store, id)
	if err != nil {
		fmt.Println("Like error:")
		fmt.Println(err)
	}
}

func Unlike(store *sql.DB, id string) {
	err := db.Unlike(store, id)
	if err != nil {
		fmt.Println("Unlike error:")
		fmt.Println(err)
	}
}

func AllCatelogue(store *sql.DB) []model.Song {
	songlist, err := db.AllSongs(store)
	if err != nil {
		fmt.Println("AllCatelogue error:")
		fmt.Println(err)
	}
	return songlist
}
