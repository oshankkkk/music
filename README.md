### my spotify replacement

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

#### Global

| Key | Action |
|---|---|
| `Ctrl+C` | Exit app |
| `escape` | Close popup / unfocus area / go back |
| `j` / `k` | Navigate down / up (volume down / up in playbar) |
| `h` / `l` | Navigate left / right (seek in playbar) |
| `dd` | Delete a selected playlist, or song |
| `enter` | Confirm / select / submit |
| `space` (alone) | Toggle play/pause |
| `space l` | Toggle repeat (loop) |
| `space p` | Focus sidebar |
| `space n` | Focus quick-access grid |
| `space m` | Focus mixes section |
| `space r` | Focus recently played |
| `space b` | Focus playbar (console) |
| `space s` | Open song search (play) |
| `space i` | Open playlist song search |
| `space q` | Open queue popup |
| `space a` | Open add-to-playlist popup |

#### Sidebar

| Key | Action |
|---|---|
| `n` | Create new playlist |
| `r` | Rename selected playlist |

#### Playbar / Console

| Key | Action |
|---|---|
| `H` / `L` | Previous / next track |
| `f` | Fast forward (60s) |
| `b` | Rewind (60s) |


