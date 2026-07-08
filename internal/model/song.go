package model
type Song struct {
	ID          string  
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
