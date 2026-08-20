### my spotify replacement

# TODO:
- make recent songs work with song handler and all of that and make sure responses are parsed in the tui
- make playlist work 
- make queue work, once a song is played and if its a playlist then add the all the songs into a app.songqueue and play one by one.
    - make db function to store the queue only once the program is closed.(snapshot of the queue saved in the db when the app closes)..
- Get the genius API to get album art, album details, artist details, search on genius and then get the url from ytdlp and run it with mpv


```
./build.sh

and 

nix develop
cd tui
bun run dev
```

```
JSON RPC schema

Request:  {"jsonrpc":"2.0","method":"play-/playlist-/queue-/","params":{},"id":}
Response: {"jsonrpc":"2.0",type"mpv/mpv-event/song/playlist/queue/unknown,"response":{},"id":}

```

```
TODO:
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


