```
        ┌──────────────┐
        │   USER UI    │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │  APP LOGIC   │
        │ (queue, etc) │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │   SEARCH     │
        │  (yt-dlp)    │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │  CACHE LAYER │
        │ SQLite + FS  │
        └──────┬───────┘
               ↓
        ┌──────────────┐
        │     mpv      │
        │ playback eng │
        └──────-───────┘

```

```
go-music/
├── cmd/
│   └── music/
│       └── main.go          # Entry point
│
├── internal/
│   ├── app/
│   │   └── app.go           # Coordinates everything
│   │
│   ├── player/
│   │   ├── mpv.go           # mpv wrapper
│   │   └── ipc.go           # (Later) mpv JSON IPC
│   │
│   ├── youtube/
│   │   ├── search.go        # yt-dlp search
│   │   ├── stream.go        # Stream URL extraction
│   │   └── download.go      # (Later) Download/cache
│   │
│   ├── queue/
│   │   └── queue.go         # Song queue
│   │
│   ├── cache/
│   │   ├── cache.go         # Cache manager
│   │   ├── storage.go       # File management
│   │   └── lru.go           # Cache eviction
│   │
│   ├── library/
│   │   └── library.go       # Offline songs
│   │
│   ├── database/
│   │   ├── db.go            # SQLite connection
│   │   └── songs.go         # Song metadata
│   │
│   ├── models/
│   │   └── song.go
│   │
│   └── config/
│       └── config.go
│
├── assets/
│
├── cache/
│   ├── audio/
│   └── thumbnails/
│
├── downloads/
│
├── data/
│   └── music.db
│
├── configs/
│   └── config.json
│
├── scripts/
│
├── go.mod
├── go.sum
└── README.md

```
