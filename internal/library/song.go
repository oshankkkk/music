package library
type Genre string

type Song struct{
	ID int
	Name string		
	Genre Genre
	PlayCount int
	isLiked bool

}
