import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";

export function RecentlyPlayed({ isFocused }: { isFocused: boolean }) {
  const [songs, setSongs] = useState([
    { id: 1, title: "Shape of You", artist: "Ed Sheeran", duration: "3:53", albumArtColor: "#ff6347" },
    { id: 2, title: "Blinding Lights", artist: "The Weeknd", duration: "3:20", albumArtColor: "#4169e1" },
    { id: 3, title: "Dance Monkey", artist: "Tones and I", duration: "3:29", albumArtColor: "#1DB954" },
    { id: 4, title: "Rockstar", artist: "Post Malone", duration: "3:38", albumArtColor: "#7b68ee" },
  ]);
  
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isFocused) return;
    if (key.name === "j") setSelectedIndex((prev) => Math.min(prev + 1, songs.length - 1));
    if (key.name === "k") setSelectedIndex((prev) => Math.max(prev - 1, 0));
    if (key.name === "d") {
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
    <box flexDirection="column" width="100%">
      <box flexDirection="column" gap={0} paddingBottom={1}>
        <text fg="#ffffff"><b>Recently Played</b></text>
        <text fg="#888888">Your recent listening history</text>
      </box>
      
      <box flexDirection="row" width="100%" paddingBottom={1} paddingX={1}>
        <text fg="#b3b3b3" width={4}>#</text>
        <text fg="#b3b3b3" width={30}>Title</text>
        <text fg="#b3b3b3" width={20}>Artist</text>
        <text fg="#b3b3b3" width={10}>Duration</text>
      </box>
      
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
