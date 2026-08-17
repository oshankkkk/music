import { useState, useRef, useCallback, useEffect } from "react";
import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { PlaylistDetail } from "./components/PlaylistDetail";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";
import { SearchPopup } from "./components/SearchPopup";
import { CreatePlaylistPopup } from "./components/CreatePlaylistPopup";
import { QueuePopup } from "./components/QueuePopup";
import { SelectPlaylistPopup } from "./components/SelectPlaylistPopup";
import type { Song, PlaylistInfo, QueueItem, RecentlyPlayedItem } from "./client/types";
import { startReader } from "./client/client";

const LEADER_TIMEOUT_MS = 300;

function App() {
  const [song, setSong] = useState<Song>({
    id: "0",
    title: "Unknown song",
    description: "Rick Astley's greatest hit",
    artist: "Unknown artist",
    timestamp: 0,
    duration: 0,
    albumArtUrl: "https://example.com/art.jpg",
    isLiked: true,
    playlists: [1, 2, 3],
    isPlayed: false
  });
  const renderer = useRenderer();
  const [playlists, setPlaylists] = useState<PlaylistInfo[]>([
    { id: "1", name: "Liked Songs" },
    { id: "2", name: "Daily Mix 1" },
    { id: "3", name: "Discover Weekly" },
  ]);
  const [queue, setQueue] = useState<QueueItem[]>([
    { queueId: "q1", songId: "1", name: "Bohemian Rhapsody" },
    { queueId: "q2", songId: "2", name: "Hotel California" },
    { queueId: "q3", songId: "3", name: "Stairway to Heaven" },
    { queueId: "q4", songId: "4", name: "Imagine" },
  ]);
  const [recentlyPlayed, setRecentlyPlayed] = useState<RecentlyPlayedItem[]>([
    { songId: "1", name: "Shape of You", isLiked: true },
    { songId: "2", name: "Blinding Lights", isLiked: false },
    { songId: "3", name: "Dance Monkey", isLiked: true },
    { songId: "4", name: "Rockstar", isLiked: false },
  ]);
  const [selectedPlaylist, setSelectedPlaylist] = useState<PlaylistInfo | null>(null);
  const [focusArea, setFocusArea] = useState<"none" | "sidebar" | "quick-access" | "mixes" | "playlist" | "recently-played">("none");
  const [isSearchOpen, setIsSearchOpen] = useState(false);
  const [isCreatePlaylistOpen, setIsCreatePlaylistOpen] = useState(false);
  const [isQueueOpen, setIsQueueOpen] = useState(false);
  const [isAddSongOpen, setIsAddSongOpen] = useState(false);
  const [isPlaying, setIsPlaying] = useState(false);
  // Track when space was pressed (timestamp), not a setTimeout handle
  const spaceTimestampRef = useRef<number | null>(null);

  // State-based trigger for deferred play toggles (safe for React/OpenTUI lifecycle)
  const [playToggleTick, setPlayToggleTick] = useState(0);
  const isFirstTickRef = useRef(true);

  // When playToggleTick changes (and it's not the initial mount), toggle play
  useEffect(() => {
    import("./client/client").then(({ startReader, addRpcListener, rpcCall }) => {
      startReader();
      
      const unsubscribeSong = addRpcListener("song", (data) => {
        setSong((prevSong) => ({
          ...prevSong,
          id: data.response.songid || prevSong.id,
          title: data.response.title || prevSong.title,
          artist: data.response.artist || prevSong.artist,
          duration: data.response.duration || prevSong.duration,
          isLiked: data.response.isliked !== undefined ? data.response.isliked : prevSong.isLiked,
          isPlayed: true,
        }));
      });
      
      const unsubscribeMpvEvent = addRpcListener("mpv-event", (data) => {
        if (data.response.event === "property-change" && data.response.name === "time-pos") {
          setSong((prevSong) => ({
            ...prevSong,
            timestamp: data.response.data !== undefined ? data.response.data : prevSong.timestamp
          }));
        } else if (data.response.event === "end-file") {
          setSong((prevSong) => ({
            ...prevSong,
            timestamp: 0,
            isPlayed: false
          }));
        }
      });
      
      const unsubscribeMpvReply = addRpcListener("mpv-reply", (data) => {
        if (data.response.request_id == "2") {
          setSong((prevSong) => ({
            ...prevSong,
            duration: data.response.data || prevSong.duration,
          }));
        }
      });
      
      const unsubscribePlaylist = addRpcListener("playlist", (data) => {
        if (data.response.method === "lib-getallplaylists" && data.response.success && data.response.playlists) {
          setPlaylists(data.response.playlists.map((pl: any) => ({
            id: String(pl.id),
            name: pl.title
          })));
        } else if (data.response.method === "lib-createplaylist" && data.response.success) {
          // Re-fetch all playlists to update the list
          rpcCall("lib-getallplaylists", "lib", {});
        } else if (data.response.method === "lib-removeplaylist" && data.response.success) {
          rpcCall("lib-getallplaylists", "lib", {});
        }
      });

      // Fetch playlists on mount
      rpcCall("lib-getallplaylists", "lib", {});

      return () => {
        unsubscribeSong();
        unsubscribeMpvEvent();
        unsubscribeMpvReply();
        unsubscribePlaylist();
      };
    });
  }, []);

  useEffect(() => {
    if (isFirstTickRef.current) {
      isFirstTickRef.current = false;
      return;
    }
    import("./client/client").then(({ rpcCall }) => {
      rpcCall("player-toggle-pause","mpv")
	  //.catch(console.error);
    });
    setIsPlaying((p) => !p);
  }, [playToggleTick]);

  // Safe deferred toggle: bumps a state counter so the actual toggle
  // happens inside a React useEffect, not a raw setTimeout
  const deferredTogglePlay = useCallback(() => {
    setPlayToggleTick((t) => t + 1);
  }, []);

  const togglePlay = useCallback(() => {
    deferredTogglePlay();
  }, [deferredTogglePlay]);

  // Poll for expired pending space presses.
  // This replaces the setTimeout-based approach that crashed when
  // togglePlay was called outside the renderer's event processing cycle.
  useEffect(() => {
    const intervalId = setInterval(() => {
      if (
        spaceTimestampRef.current !== null &&
        Date.now() - spaceTimestampRef.current >= LEADER_TIMEOUT_MS
      ) {
        spaceTimestampRef.current = null;
        deferredTogglePlay();
      }
    }, 50);
    return () => clearInterval(intervalId);
  }, [deferredTogglePlay]);

  useKeyboard((key) => {
    // Ctrl+C exits the app
    if (key.ctrl && key.name === "c") {
      renderer.destroy();
      process.exit(0);
    }

    // Escape unfocuses (resets to playbar) and clears pending leader
    if (key.name === "escape") {
      spaceTimestampRef.current = null;
      if (isSearchOpen) {
        setIsSearchOpen(false);
      } else if (isCreatePlaylistOpen) {
        setIsCreatePlaylistOpen(false);
      } else if (isQueueOpen) {
        setIsQueueOpen(false);
      } else if (isAddSongOpen) {
        setIsAddSongOpen(false);
      } else if (focusArea !== "none") {
        setFocusArea("none");
      } else if (selectedPlaylist) {
        setSelectedPlaylist(null);
        setFocusArea("sidebar");
      }
      return;
    }

    if (isSearchOpen || isCreatePlaylistOpen || isQueueOpen || isAddSongOpen) {
      return;
    }

    // Handle 'n' for new playlist when sidebar is focused
    if (focusArea === "sidebar" && key.name === "n") {
      setIsCreatePlaylistOpen(true);
      return;
    }

    // If there's a pending space, check for sequence completion
    if (spaceTimestampRef.current !== null) {
      spaceTimestampRef.current = null;

      switch (key.name) {
        case "p": setFocusArea("sidebar"); return;
        case "n": setFocusArea("quick-access"); return;
        case "m": setFocusArea("mixes"); return;
        case "r": setFocusArea("recently-played"); return;
        case "b": setFocusArea("none"); return;
        case "s": setIsSearchOpen(true); return;
        case "q": setIsQueueOpen(true); return;
        case "a": setIsAddSongOpen(true); return;
        default:
          // Not a sequence key — fire the buffered space as play/pause
          togglePlay();
          break; // fall through to handle current key normally
      }
    }

    // Buffer space for potential leader sequence
    if (key.name === "space") {
      spaceTimestampRef.current = Date.now();
      return;
    }
  });

  return (
    <box flexDirection="column" width="100%" height="100%" backgroundColor="#000000">
      <box width="100%" height={1} justifyContent="center" alignItems="center" backgroundColor="#181818">
        <text fg="#b3b3b3">Search: space+s | Add to Playlist: space+a | Exit: ctrl+c</text>
      </box>
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar 
          isFocused={focusArea === "sidebar" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen} 
          playlists={playlists} 
          setPlaylists={setPlaylists}
          onSelectPlaylist={(p) => { setSelectedPlaylist(p); setFocusArea("playlist"); }}
          onDeletePlaylist={(id) => {
            import("./client/client").then(({ rpcCall }) => {
              rpcCall("lib-removeplaylist", "lib", { playlistid: parseInt(id) });
            });
          }}
        />
        {selectedPlaylist ? (
          <PlaylistDetail 
            playlist={selectedPlaylist} 
            isFocused={focusArea === "playlist" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen} 
            onBack={() => { setSelectedPlaylist(null); setFocusArea("sidebar"); }} 
          />
        ) : (
          <MainContent focusArea={isSearchOpen || isCreatePlaylistOpen || isQueueOpen || isAddSongOpen ? "none" : focusArea} recentlyPlayed={recentlyPlayed} />
        )}
        <ContextPanel />
      </box>
      <Playbar isFocused={focusArea === "none" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen} isPlaying={isPlaying} onTogglePlay={togglePlay} song={song} setSong={setSong} />
      <SearchPopup isOpen={isSearchOpen}/>
      <CreatePlaylistPopup 
        isOpen={isCreatePlaylistOpen} 
        onClose={() => setIsCreatePlaylistOpen(false)} 
        onSubmit={(name) => {
          import("./client/client").then(({ rpcCall }) => {
            rpcCall("lib-createplaylist", "lib", { title: name });
          });
        }} 
      />
      <QueuePopup isOpen={isQueueOpen} onClose={() => setIsQueueOpen(false)} queue={queue} setQueue={setQueue} />
      <SelectPlaylistPopup
        isOpen={isAddSongOpen}
        playlists={playlists}
        onClose={() => setIsAddSongOpen(false)}
        onSelect={(playlistId) => {
          import("./client/client").then(({ rpcCall }) => {
            rpcCall("lib-addsongtoplaylist", "lib", { playlistid: parseInt(playlistId), songid: parseInt(song.id) });
            // optionally refresh selected playlist if it's currently open
            if (selectedPlaylist && selectedPlaylist.id === playlistId) {
              rpcCall("lib-getplaylistsongs", "lib", { playlistid: parseInt(playlistId) });
            }
          });
          setIsAddSongOpen(false);
        }}
      />
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);

