### my spotify replacement


### edgecases
1. what if i run 2 songs backtoback
2. when i delete the playlist but im still init in the tui
3. open and close together  

> Get the genius API to get album art, album details, artist details, search on genius and then get the url from ytdlp and run it with mpv

```
nix develop
./build.sh
```

```
JSON RPC schema

Request:  {"jsonrpc":"2.0","method":"play-/playlist-/queue-/","params":{},"id":}
Response: {"jsonrpc":"2.0",type"mpv/mpv-event/song/playlist/queue/unknown,"response":{},"id":}

```

### Keybinding Reference

| Sequence | Action | Scope |
|---|---|---|
| `space` (alone) | Toggle play/pause | Global |
| `space p` | Focus sidebar | Global |
| `space n` | Focus quick-access grid | Global |
| `space m` | Focus mixes section | Global |
| `space b` | Focus bottom playbar | Global |
| `escape` | Unfocus / exit app | Global |
| `Ctrl+C` | Exit app | Global |
| `j/k` | Navigate down/up | Sidebar, Quick-access |
| `h/l` | Navigate left/right | Quick-access, Mixes |
| `j/k` | Volume down/up | Playbar focused |
| `h/l` | Prev/next track | Playbar focused |


