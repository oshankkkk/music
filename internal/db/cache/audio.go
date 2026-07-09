package cache

import (
	"database/sql"
	"time"

	_ "github.com/mattn/go-sqlite3"
	//"music/internal/model"
)

func AddToCache(db *sql.DB, id string, filepath string, expiresAt time.Time) error {
	var expiresAtVal interface{}
	if expiresAt.IsZero() {
		expiresAtVal = nil
	} else {
		expiresAtVal = expiresAt.Unix()
	}

	_, err := db.Exec(
		`INSERT INTO audioCache (id, filepath, created_at, expires_at)
		 VALUES (?, ?, ?, ?)
		 ON CONFLICT(id) DO UPDATE SET
		   filepath = excluded.filepath,
		   created_at = excluded.created_at,
		   expires_at = excluded.expires_at`,
		id, filepath, time.Now().Unix(), expiresAtVal,
	)
	return err
}

func GetCacheAudio(db *sql.DB, id string) (string, error) {
	var filepath string
	var expiresAt sql.NullInt64

	err := db.QueryRow(
		`SELECT filepath, expires_at FROM audioCache WHERE id = ?`,
		id,
	).Scan(&filepath, &expiresAt)
	if err != nil {
		return "", err 
		// includes sql.ErrNoRows if missing
	}

	if expiresAt.Valid && time.Now().Unix() > expiresAt.Int64 {
		return "", sql.ErrNoRows 
		// treat expired entries as not found
	}

	return filepath, nil
}

func CheckCacheAudio(db *sql.DB, id string) bool {
	_, err := GetCacheAudio(db, id)
	return err == nil
}
