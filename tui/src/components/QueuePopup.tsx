import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";

export function QueuePopup({ isOpen, onClose }: { isOpen: boolean, onClose: () => void }) {
  const [songs, setSongs] = useState([
    { id: 1, title: "Bohemian Rhapsody", artist: "Queen", albumArtColor: "#ff6347" },
    { id: 2, title: "Hotel California", artist: "Eagles", albumArtColor: "#4169e1" },
    { id: 3, title: "Stairway to Heaven", artist: "Led Zeppelin", albumArtColor: "#1DB954" },
    { id: 4, title: "Imagine", artist: "John Lennon", albumArtColor: "#7b68ee" },
  ]);
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isOpen) return;

    if (key.name === "escape") {
      onClose();
      return;
    }

    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, songs.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "d") {
      const now = Date.now();
      if (now - lastDPress.current < 500) {
        // Double D pressed! Remove the selected song
        if (songs.length > 0) {
            setSongs((prev) => {
                const newSongs = prev.filter((_, i) => i !== selectedIndex);
                if (selectedIndex >= newSongs.length) {
                    setSelectedIndex(Math.max(0, newSongs.length - 1));
                }
                return newSongs;
            });
        }
        lastDPress.current = 0; // reset
      } else {
        lastDPress.current = now;
      }
    }
  });

  if (!isOpen) return null;

  return (
    <box
      position="absolute"
      width="100%"
      height="100%"
      backgroundColor="#000000"
      flexDirection="column"
      padding={2}
    >
      <text fg="#ffffff"><b>Play Queue</b></text>
      <text fg="#888888">Press 'DD' to remove a song. 'Escape' to close.</text>
      
      <box flexDirection="column" marginTop={1} gap={1} flexGrow={1}>
        {songs.map((song, i) => {
          const isSelected = selectedIndex === i;
          return (
            <box key={song.id} flexDirection="row" gap={2} alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
              <text fg={song.albumArtColor}>██</text>
              <box flexDirection="column">
                <text fg={isSelected ? "#1DB954" : "#ffffff"}>{song.title}</text>
                <text fg="#b3b3b3">{song.artist}</text>
              </box>
            </box>
          );
        })}
      </box>
    </box>
  );
}
