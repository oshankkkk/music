package library
type Song struct {
	ID          string  `json:"id"`
	Title       string  `json:"title"`
	Artist      string  `json:"channel"`
	Duration    float64 `json:"duration"`
	Thumbnail   string  `json:"thumbnail"`
	ViewCount   int64   `json:"view_count"`
	UploadDate  string  `json:"upload_date"`
	URL         string  `json:"url"`
	isLiked bool
	PersonalPlayCount int
	Genre string
}


