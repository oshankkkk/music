CREATE TABLE song (
    id TEXT PRIMARY KEY,
    title TEXT NOT NULL,
    artist TEXT NOT NULL,
    duration INTEGER,
    isLiked BOOLEAN NOT NULL,
	lastPlayed TEXT
);

ALTER TABLE song
ADD COLUMN genre TEXT;


