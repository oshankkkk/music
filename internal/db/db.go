// make a db(put a fitting name) struct and make a feild called dp and pass the *sql.DB and make all the 
//methods that uses that value the structs recievers so we do not have to explicitly pass them into those funcitons.

package db
import (
	"database/sql"
	"errors"
	"fmt"

	"music/internal/model"
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
func RemoveSong(db *sql.DB, id string) error {
	exists, err := CheckSong(db, id)
	if err != nil {
		return err
	}
	if !exists {
		return errors.New("song not found")
	}

	query := `DELETE FROM song WHERE id=?`
	_, err = db.Exec(query, id)
	return err
}

func CheckSong(db *sql.DB, id string) (bool, error) {
	var exists bool
	query := `SELECT EXISTS(SELECT 1 FROM song WHERE id=?)`
	err := db.QueryRow(query, id).Scan(&exists)
	return exists, err
}

func AddSong(db *sql.DB, s model.Song) error{
	query := `
	INSERT INTO song (id,title, artist, duration, isliked, genre)
	VALUES (?,?, ?, ?, ?, ?)
	`
	_, err := db.Exec(
		query,
		s.ID,
		s.Title,
		s.Artist,
		s.Duration,
		s.IsLiked,
		s.Genre,
	)
	if err != nil {
		return err
	}
	return  nil

}

func GetSong(db *sql.DB, id string) (model.Song, error) {
	var s model.Song
	query := `SELECT id, title, artist, duration, isliked, genre FROM song WHERE id=?`
	fmt.Println("song id",id)
	err := db.QueryRow(query, id).Scan(&s.ID, &s.Title, &s.Artist, &s.Duration, &s.IsLiked, &s.Genre)
	if err == sql.ErrNoRows {
		return model.Song{}, errors.New("song not found")
	}
	if err != nil {
		return model.Song{}, err
	}
	return s, nil
}

func Play(db *sql.DB, id string) error {
	query := `UPDATE song SET playcount = playcount + 1 WHERE id=?`
	res, err := db.Exec(query, id)
	if err != nil {
		return err
	}
	rows, err := res.RowsAffected()
	if err != nil {
		return err
	}
	if rows == 0 {
		return errors.New("song not found")
	}
	return nil
}

func Like(db *sql.DB, id string) error {
	query := `UPDATE song SET isliked=? WHERE id=?`
	res, err := db.Exec(query, true, id)
	if err != nil {
		return err
	}
	rows, err := res.RowsAffected()
	if err != nil {
		return err
	}
	if rows == 0 {
		return errors.New("song not found")
	}
	return nil
}

func Unlike(db *sql.DB, id string) error {
	query := `UPDATE song SET isliked=? WHERE id=?`
	res, err := db.Exec(query, false, id)
	if err != nil {
		return err
	}
	rows, err := res.RowsAffected()
	if err != nil {
		return err
	}
	if rows == 0 {
		return errors.New("song not found")
	}
	return nil
}

func AllSongs(db *sql.DB) ([]model.Song, error) {
	query := `SELECT id, title, artist, duration, isliked, genre FROM song`
	rows, err := db.Query(query)
	if err != nil {
		return nil, err
	}
	defer rows.Close()

	var songs []model.Song
	for rows.Next() {
		var s model.Song
		if err := rows.Scan(&s.ID, &s.Title, &s.Artist, &s.Duration, &s.IsLiked, &s.Genre); err != nil {
			return nil, err
		}
		songs = append(songs, s)
	}
	return songs, rows.Err()
}
