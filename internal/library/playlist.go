package library


type Playlist struct{
ID string
Title string
Duration string
Songs []Song
SongCount int
CreatedDate string
}

func CreatePlaylist(song Song){
var playlist []Song
playlist = append(playlist, song)

}
func DeletePylist(id string){}
func AddToPylist(id string){
	
}
func RemoveFromPylist(){

}


