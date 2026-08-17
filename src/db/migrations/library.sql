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

CREATE TABLE catalogue (
    id INTEGER PRIMARY KEY,
    songID INTEGER NOT NULL,
    playlistID INTEGER NOT NULL,
    FOREIGN KEY (songID) REFERENCES song(id),
    FOREIGN KEY (playlistID) REFERENCES playlist(id)
);

CREATE TABLE playlist (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    duration INTEGER,
    song_count INTEGER NOT NULL DEFAULT 0,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE collection (
    collection_id INTEGER PRIMARY KEY AUTOINCREMENT,
    playlist_id INTEGER NOT NULL,
    song_id INTEGER NOT NULL,
    FOREIGN KEY (playlist_id) REFERENCES playlist(id)
);

CREATE TABLE queue (
    queue_id INTEGER PRIMARY KEY AUTOINCREMENT,
    song_id INTEGER NOT NULL,
    position INTEGER NOT NULL
);

