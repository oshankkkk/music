package library

import (
	"errors"
)

type Song struct {
	ID          string  `json:"id"`
	Title       string  `json:"title"`
	Artist      string  `json:"channel"`
	Duration    float64 `json:"duration"`
	Thumbnail   string  `json:"thumbnail"`
	ViewCount   int64   `json:"view_count"`
	UploadDate  string  `json:"upload_date"`
	URL         string  `json:"url"`
	IsLiked bool
	PersonalPlayCount int
	Genre string
}


func CheckSong(id string, catalogue map[string]*Song) bool {
	_, ok := catalogue[id]
	return ok
}

func  AddSong(song Song, Catelogue map[string]*Song) Song {

	Catelogue[song.ID] = &song
	return song
}

func RemoveSong(id string ,Catelogue map[string]*Song) error {
	if !CheckSong(id,Catelogue) {
		return errors.New("song not found")
	}
	delete(Catelogue, id)
	return nil
}

func  GetSong(id string ,Catelogue map[string]*Song) (Song, error) {
	s, ok := Catelogue[id]
	if !ok {
		return Song{}, errors.New("song not found")
	}
	return *s, nil
}

func  Play(id string, Catelogue map[string]*Song) error {
	s, ok := Catelogue[id]
	if !ok {
		return errors.New("song not found")
	}
	s.PersonalPlayCount++
	return nil
}

func  Like(id string, Catelogue map[string]*Song) error {
	s, ok := Catelogue[id]
	if !ok {
		return errors.New("song not found")
	}
	s.IsLiked = true
	return nil
}

func  Unlike(id string, Catelogue map[string]*Song) error {
	s, ok := Catelogue[id]
	if !ok {
		return errors.New("song not found")
	}
	s.IsLiked = false
	return nil
}

func ACatelogue( Catelogue map[string]*Song) []Song {
	songs := make([]Song, 0, len(Catelogue))
	for _, s := range Catelogue {
		songs = append(songs, *s)
	}
	return songs
}

