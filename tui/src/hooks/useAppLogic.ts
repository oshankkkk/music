import { useState, useRef, useCallback, useEffect } from "react";
import { useKeyboard, useRenderer } from "@opentui/react";
import type { Song, PlaylistInfo, QueueItem, RecentlyPlayedItem } from "../client/types";
import { addRpcListener, rpcCall } from "../client/client";

const LEADER_TIMEOUT_MS = 300;

export function useAppLogic() {
  const [song, setSong] = useState<Song>({
    id: "0",
    title: "Unknown song",
    description: "",
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
  const [queue, setQueue] = useState<QueueItem[]>([]);
  const [recentlyPlayed, setRecentlyPlayed] = useState<RecentlyPlayedItem[]>([]);
  const [selectedPlaylist, setSelectedPlaylist] = useState<PlaylistInfo | null>(null);
  const [focusArea, setFocusArea] = useState<"none" | "sidebar" | "quick-access" | "mixes" | "playlist" | "recently-played">("none");
  const [isSearchOpen, setIsSearchOpen] = useState(false);
  const [isCreatePlaylistOpen, setIsCreatePlaylistOpen] = useState(false);
  const [isQueueOpen, setIsQueueOpen] = useState(false);
  const [isAddSongOpen, setIsAddSongOpen] = useState(false);
  const [isPlaying, setIsPlaying] = useState(false);
  const [volume, setVolume] = useState(50);

  const [isPlaylistSearchOpen, setIsPlaylistSearchOpen] = useState(false);
  const [playlistToRename, setPlaylistToRename] = useState<PlaylistInfo | null>(null);
  const [isRepeat, setIsRepeat] = useState(false);

  const spaceTimestampRef = useRef<number | null>(null);
  const [playToggleTick, setPlayToggleTick] = useState(0);
  const isFirstTickRef = useRef(true);
  const isInitialQueueLoad = useRef(true);

  const toggleRepeat = useCallback(async () => {
    try {
      await rpcCall("player-toggle-repeat", "player");
      setIsRepeat((r) => !r);
    } catch (e) {
      console.error(e);
    }
  }, []);

  useEffect(() => {
    const unsubscribeSong = addRpcListener("song", (data) => {
      if (data.response.method === "song-playSong") {
        const songData = data.response.songlist;
        if (songData) {
          rpcCall("lib-getqueuesongs", "queue", {});
          setSong((prevSong) => ({
            ...prevSong,
            id: songData.songid || prevSong.id,
            title: songData.title || prevSong.title,
            artist: songData.artist || prevSong.artist,
            duration: songData.duration !== undefined ? songData.duration : prevSong.duration,
            isLiked: songData.isliked !== undefined ? songData.isliked : prevSong.isLiked,
            isPlayed: true,
          }));
        }
      } else if (data.response.method === "song-likesong" || data.response.method === "song-unlikesong") {
        if (data.response.success) {
          setSong((prevSong) => ({
            ...prevSong,
            isLiked: data.response.method === "song-likesong",
          }));
        }
      } else if (data.response.method === "song-getallsongs" && data.response.success && data.response.songlist) {
        setRecentlyPlayed(data.response.songlist.map((s: any) => ({
          songId: s.id,
          name: s.title || s.artist || "Unknown",
          artist: s.artist || "Unknown",
          isLiked: s.isliked || false
        })));
      }
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
      } else if (typeof data.response.data === "number") {
        setVolume(data.response.data);
      }
    });
    
    const unsubscribePlaylist = addRpcListener("playlist", (data) => {
      if (data.response.method === "lib-getallplaylists" && data.response.success && data.response.playlists) {
        setPlaylists(data.response.playlists.map((pl: any) => ({
          id: String(pl.id),
          name: pl.title
        })));
      } else if (data.response.method === "lib-createplaylist" && data.response.success) {
        rpcCall("lib-getallplaylists", "lib", {});
      } else if ((data.response.method === "lib-removeplaylist" || data.response.method === "lib-renameplaylist") && data.response.success) {
        rpcCall("lib-getallplaylists", "lib", {});
      }
    });

    const unsubscribeQueue = addRpcListener("queue", (data) => {
      if (data.response.method === "lib-getqueuesongs" && data.response.success && data.response.queuelist) {
        setQueue(data.response.queuelist.map((sq: any, idx: number) => ({
          queueId: String(idx),
          songId: sq.songid,
          name: sq.title || sq.artist || "Unknown"
        })));

        if (isInitialQueueLoad.current) {
          isInitialQueueLoad.current = false;
          if (data.response.queuelist.length > 0) {
            const topItem = data.response.queuelist[0];
            setSong((prevSong) => ({
              ...prevSong,
              id: topItem.songid || prevSong.id,
              title: topItem.title || prevSong.title,
              artist: topItem.artist || prevSong.artist,
              isLiked: topItem.isliked !== undefined ? topItem.isliked : prevSong.isLiked,
            }));
          }
        }
      } else if (data.response.method === "lib-addtoqueue" || data.response.method === "lib-removefromqueue" || data.response.method === "lib-clearqueue") {
        if (data.response.success) {
          rpcCall("lib-getqueuesongs", "queue", {});
        }
      }
    });

    rpcCall("lib-getallplaylists", "lib", {});
    rpcCall("lib-getqueuesongs", "queue", {});
    rpcCall("player-get-volume", "player", {});
    rpcCall("song-getallsongs", "song", {});

    return () => {
      unsubscribeSong();
      unsubscribeMpvEvent();
      unsubscribeMpvReply();
      unsubscribePlaylist();
      unsubscribeQueue();
    };
  }, []);

  useEffect(() => {
    if (isFirstTickRef.current) {
      isFirstTickRef.current = false;
      return;
    }
    rpcCall("player-toggle-pause","player");
    setIsPlaying((p) => !p);
  }, [playToggleTick]);

  const deferredTogglePlay = useCallback(() => {
    setPlayToggleTick((t) => t + 1);
  }, []);

  const togglePlay = useCallback(() => {
    deferredTogglePlay();
  }, [deferredTogglePlay]);

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
    if (key.ctrl && key.name === "c") {
      renderer.destroy();
      process.exit(0);
    }

    if (key.name === "escape") {
      spaceTimestampRef.current = null;
      if (isSearchOpen) {
        setIsSearchOpen(false);
      } else if (isPlaylistSearchOpen) {
        setIsPlaylistSearchOpen(false);
      } else if (isCreatePlaylistOpen) {
        setIsCreatePlaylistOpen(false);
      } else if (isQueueOpen) {
        setIsQueueOpen(false);
      } else if (isAddSongOpen) {
        setIsAddSongOpen(false);
      } else if (playlistToRename) {
        setPlaylistToRename(null);
      } else if (focusArea !== "none") {
        setFocusArea("none");
      } else if (selectedPlaylist) {
        setSelectedPlaylist(null);
        setFocusArea("sidebar");
      }
      return;
    }

    if (isSearchOpen || isCreatePlaylistOpen || isQueueOpen || isAddSongOpen || isPlaylistSearchOpen || playlistToRename) {
      return;
    }

    if (focusArea === "sidebar" && key.name === "n") {
      setIsCreatePlaylistOpen(true);
      return;
    }

    if (spaceTimestampRef.current !== null) {
      spaceTimestampRef.current = null;

      switch (key.name) {
        case "p": setFocusArea("sidebar"); return;
        case "n": setFocusArea("quick-access"); return;
        case "m": setFocusArea("mixes"); return;
        case "r": setFocusArea("recently-played"); return;
        case "b": setFocusArea("none"); return;
        case "s": setIsSearchOpen(true); return;
        case "i": setIsPlaylistSearchOpen(true); return;
        case "q": setIsQueueOpen(true); return;
        case "a": setIsAddSongOpen(true); return;
        case "y": toggleRepeat(); return;
        default:
          togglePlay();
          break;
      }
    }

    if (key.name === "space") {
      spaceTimestampRef.current = Date.now();
      return;
    }
  });

  return {
    song, setSong,
    playlists, setPlaylists,
    queue, setQueue,
    recentlyPlayed, setRecentlyPlayed,
    selectedPlaylist, setSelectedPlaylist,
    focusArea, setFocusArea,
    isSearchOpen, setIsSearchOpen,
    isPlaylistSearchOpen, setIsPlaylistSearchOpen,
    isCreatePlaylistOpen, setIsCreatePlaylistOpen,
    playlistToRename, setPlaylistToRename,
    isQueueOpen, setIsQueueOpen,
    isAddSongOpen, setIsAddSongOpen,
    isPlaying, togglePlay,
    volume, setVolume,
    isRepeat, toggleRepeat
  };
}
