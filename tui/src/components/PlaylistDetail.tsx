import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";
import type { PlaylistInfo } from "../client/types";

type SongInfo = { id: number; title: string; artist: string; duration: string; albumArtColor: string; };

export function PlaylistDetail({ playlist, isFocused, onBack }: { playlist: PlaylistInfo, isFocused: boolean, onBack: () => void }) {
  const [songs, setSongs] = useState<SongInfo[]>([
    { id: 1, title: "Song 1", artist: "Artist 1", duration: "3:45", albumArtColor: "#ff6347" },
    { id: 2, title: "Song 2", artist: "Artist 2", duration: "4:20", albumArtColor: "#4169e1" },
    { id: 3, title: "Song 3", artist: "Artist 3", duration: "2:55", albumArtColor: "#1DB954" },
    { id: 4, title: "Song 4", artist: "Artist 4", duration: "5:10", albumArtColor: "#7b68ee" },
    { id: 5, title: "Song 5", artist: "Artist 5", duration: "3:30", albumArtColor: "#ff1493" },
  ]);
  
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isFocused) return;
    
    // Add escape to go back
    if (key.name === "escape") {
      onBack();
      return;
    }

    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, songs.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "d") {
      const now = Date.now();
      if (now - lastDPress.current < 500) {
        if (songs.length > 0) {
            setSongs((prev) => {
                const newSongs = prev.filter((_, i) => i !== selectedIndex);
                if (selectedIndex >= newSongs.length) {
                    setSelectedIndex(Math.max(0, newSongs.length - 1));
                }
                return newSongs;
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
    </box>
  );
}
