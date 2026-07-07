package db

import (
	"database/sql"
	"fmt"

	"music/internal/library"

	_ "github.com/mattn/go-sqlite3"
)

func check(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func InitDB(path string) (*sql.DB, error) {
	db, err := sql.Open("sqlite3", path)
	if err != nil {
		return nil, err
	}
	if err := db.Ping(); err != nil {
		db.Close()
		return nil, err
	}
	return db, nil
}

func InsertSong(db *sql.DB, s library.Song) error {
	query := `
	INSERT INTO song (title, artist, duration, isliked, genre)
	VALUES (?, ?, ?, ?, ?)
	`
	_, err := db.Exec(
		query,
		s.Title,
		s.Artist,
		s.Duration,
		s.IsLiked,
		s.Genre,
	)
	return err
}

func RemoveSong(db *sql.DB, s library.Song) error {
	query := `
	DELETE FROM song
	WHERE id=?
	`
	_, err := db.Exec(
		query,
		s.ID,
	)
	return err
}
