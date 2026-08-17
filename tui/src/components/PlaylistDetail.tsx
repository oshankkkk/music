import { useState, useRef, useEffect } from "react";
import { useKeyboard } from "@opentui/react";
import type { PlaylistInfo } from "../client/types";
import { rpcCall, addRpcListener } from "../client/client";
import { SongPlaylistAddSearch } from "./SongPlaylistAddSearch";

type SongInfo = { id: number; title: string; artist: string; duration: string; albumArtColor: string; };

export function PlaylistDetail({ playlist, isFocused, isPlaylistSearchOpen, onBack }: { playlist: PlaylistInfo, isFocused: boolean, isPlaylistSearchOpen: boolean, onBack: () => void }) {
  const [songs, setSongs] = useState<SongInfo[]>([]);
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useEffect(() => {
    rpcCall("lib-getplaylistsongs", "lib", { playlistid: parseInt(playlist.id) });

    const unsubscribe = addRpcListener("playlist", (data) => {
      if (data.response.method === "lib-getplaylistsongs" && data.response.success && data.response.songs) {
        setSongs(data.response.songs.map((s: any) => ({
          id: parseInt(s.id),
          title: s.title || "Unknown",
          artist: s.artist || "Unknown",
          duration: s.duration ? `${Math.floor(s.duration / 60)}:${String(s.duration % 60).padStart(2, '0')}` : "0:00",
          albumArtColor: "#1DB954"
        })));
      } else if (data.response.method === "lib-deletesongfromplaylist" && data.response.success) {
        rpcCall("lib-getplaylistsongs", "lib", { playlistid: parseInt(playlist.id) });
      }
    });

    return unsubscribe;
  }, [playlist.id]);

  useKeyboard((key) => {
    if (!isFocused && !isPlaylistSearchOpen) return;
    
    if (key.name === "escape") {
      onBack();
      return;
    }

    if (isPlaylistSearchOpen) return;

    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, songs.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "d") {
      const now = Date.now();
      if (now - lastDPress.current < 500) {
        if (songs.length > 0 && songs[selectedIndex]) {
          rpcCall("lib-deletesongfromplaylist", "lib", { 
            playlistid: parseInt(playlist.id), 
            songid: songs[selectedIndex].id 
          });
        }
        lastDPress.current = 0;
      } else {
        lastDPress.current = now;
      }
    }
  });

  return (
    <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={2}>
      {/* Header */}
      <box flexDirection="row" gap={3} paddingBottom={2}>
        <box flexDirection="column">
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
            <text fg="#1DB954">██████████████</text>
        </box>
        <box flexDirection="column" justifyContent="flex-end" paddingBottom={0}>
          <text fg="#ffffff">Playlist</text>
          <text fg="#ffffff"><b>{playlist.name}</b></text>
          <text fg="#b3b3b3">Playlist • {songs.length} songs, 19 min 40 sec</text>
        </box>
      </box>
      
      {/* Table Header */}
      <box flexDirection="row" width="100%" paddingBottom={1} paddingX={1}>
        <text fg="#b3b3b3" width={4}>#</text>
        <text fg="#b3b3b3" width={30}>Title</text>
        <text fg="#b3b3b3" width={20}>Artist</text>
        <text fg="#b3b3b3" width={10}>Duration</text>
      </box>
      
      {/* Song List */}
      <box flexDirection="column" flexGrow={1} gap={0}>
        {songs.map((song, i) => {
          const isSelected = isFocused && selectedIndex === i;
          return (
            <box key={song.id} flexDirection="row" width="100%" alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
              <text fg={isSelected ? "#1DB954" : "#b3b3b3"} width={4}>{String(i + 1)}</text>
              <box flexDirection="row" gap={1} width={30} alignItems="center">
                <text fg={song.albumArtColor}>██</text>
                <text fg={isSelected ? "#1DB954" : "#ffffff"}>{song.title}</text>
              </box>
              <text fg="#b3b3b3" width={20}>{song.artist}</text>
              <text fg="#b3b3b3" width={10}>{song.duration}</text>
            </box>
          );
        })}
      </box>
      <SongPlaylistAddSearch isOpen={isPlaylistSearchOpen} />
    </box>
  );
}
