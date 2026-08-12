### my spotify replacement

```
./build.sh

and 

nix develop
cd tui
bun run dev
```

```

// Protocol: each request/response is a single JSON object, newline-terminated.
// Request:  {"jsonrpc":"2.0","method":"add","params":{"a":2,"b":3},"id":1}
// Response: {"jsonrpc":"2.0","result":5,"id":1}

```

```
//TODO for later:

//Both queue and the lib we need a better way to identify them uniquely,once made a id  is automatically generated. i want you to 
//(we need to get the id back from the db and put that in the ui so when it calls a lib or the song,
//there is a queue and a playlist table in the music.db, (go look at the migrations and read it). 
//make a clear queue function in src/lib/queue.
//make db function to store the queue only once the program is closed.(snapshot of the queue saved in the db when the app closes)..
//it reference the db identifer.
//we need a rename playlist function.
//need to fix the funciton declaration error in the dispatch.c
//need to send RPC responses back to the ui after anything happen rn its only for mpv
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


