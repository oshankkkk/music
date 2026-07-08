package library
//
//import (
//	"errors"
//	"strconv"
//	"time"
//)
//
//type Playlist struct {
//	ID          string
//	Title       string
//	Duration    string
//	Songs       []Song
//	SongCount   int
//	CreatedDate string
//}
//
//// CreatePlaylist makes a new playlist with an auto-assigned ID and stores it.
//func CreatePlaylist(title string)  {
//	playlist := Playlist{
//		ID:          "pl-" + strconv.Itoa(l.nextPlaylistID),
//		Title:       title,
//		Songs:       []Song{},
//		SongCount:   0,
//		CreatedDate: time.Now().Format("2006-01-02"),
//	}
//	// add to the db
////	l.playlists[playlist.ID] = &playlist
//	//return playlist
//}
//
//// DeletePlaylist removes a playlist by ID.
//func DeletePlaylist(id string) error {
//	if _, ok := l.playlists[id]; !ok {
//		return errors.New("playlist not found")
//	}
//	delete(l.playlists, id)
//	return nil
//}
//
//// AddToPlaylist appends a song to an existing playlist.
//func (l *Library) AddToPlaylist(id string, song Song) error {
//	p, ok := l.playlists[id]
//	if !ok {
//		return errors.New("playlist not found")
//	}
//	p.Songs = append(p.Songs, song)
//	p.SongCount = len(p.Songs)
//	return nil
//}
//
//// RemoveFromPlaylist removes a song (matched by song ID) from a playlist.
//func (l *Library) RemoveFromPlaylist(playlistID string, songID int) error {
//	p, ok := l.playlists[playlistID]
//	if !ok {
//		return errors.New("playlist not found")
//	}
//
//	for i, s := range p.Songs {
//		if s.ID == songID {
//			p.Songs = append(p.Songs[:i], p.Songs[i+1:]...)
//			p.SongCount = len(p.Songs)
//			return nil
//		}
//	}
//	return errors.New("song not found in playlist")
//}
//
//// GetPlaylist retrieves a playlist by ID.
//func (l *Library) GetPlaylist(id string) (Playlist, error) {
//	p, ok := l.playlists[id]
//	if !ok {
//		return Playlist{}, errors.New("playlist not found")
//	}
//	return *p, nil
//}
//
//// AllPlaylists returns a snapshot of every playlist currently stored.
//func (l *Library) AllPlaylists() []Playlist {
//	playlists := make([]Playlist, 0, len(l.playlists))
//	for _, p := range l.playlists {
//		playlists = append(playlists, *p)
//	}
//	return playlists
//}
//
