CREATE TABLE audioCache(
	id TEXT PRIMARY KEY,
	filepath TEXT NOT NULL,	
    created_at INTEGER NOT NULL,
    expires_at INTEGER
);


