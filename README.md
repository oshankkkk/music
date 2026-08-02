### my spotify replacement

```
gcc music.c -o musix -Wall -Werror -lsqlite3 -lcjson
```

```

// Protocol: each request/response is a single JSON object, newline-terminated.
// Request:  {"jsonrpc":"2.0","method":"add","params":{"a":2,"b":3},"id":1}
// Response: {"jsonrpc":"2.0","result":5,"id":1}

```
#### Figure out the RPC connection
#### Library
- Search YouTube (via yt-dlp)
- Stream music
- Download/cache songs
- Mark songs as favorites
- View recently played
- View most played
- Fetch album art
- Fetch lyrics
#### Queue
- Queue songs
- Clear queue
- Shuffle
- Repeat (off, one, all)
#### Playlists
- Create playlist
- Rename playlist
- Delete playlist
- Add songs to playlist
- Remove songs from playlist
- Reorder songs
- Duplicate playlist

#### User Interface
- Keyboard-driven TUI
- View now playing
- Remember playback position
#### Infrastructure
- SQLite library
- Configuration files
- Theme support
- Lua plugin system

### mpv Features 
uses mpv IPC

- Play audio
- Pause
- Resume
- Stop
- Seek (forward/backward)
- Skip to next song*
- Skip to previous song*
- Fast forward
- Rewind
- Volume control
- Mute/unmute

> **Note:** "Next" and "Previous" are handled by **your queue**, but the actual playback of the next file is still performed by mpv.

### Plugins (Lua)
- Spotify migration
- Discord Rich Presence
- Last.fm scrobbling
- MPRIS integration
- Desktop notifications

- Additional lyrics providers
- Additional album art providers
- MusicBrainz metadata lookup
- Genius integration
- Custom search providers

- Smart playlists
- Auto-DJ
- Recommendation algorithms
- Queue automation
- Playback rules (e.g. skip short songs)

- Custom keybindings
- Custom commands
- Status bar widgets
- Startup dashboard
- Themes (optional if not built into core)

- Statistics dashboards
- Tag editing
- Import/export tools
- Custom file naming for downloads




