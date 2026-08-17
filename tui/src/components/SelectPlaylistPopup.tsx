import { useState } from "react";
import { useKeyboard } from "@opentui/react";
import type { PlaylistInfo } from "../client/types";

export function SelectPlaylistPopup({ isOpen, playlists, onClose, onSelect }: { isOpen: boolean, playlists: PlaylistInfo[], onClose: () => void, onSelect: (playlistId: string) => void }) {
  const [selectedIndex, setSelectedIndex] = useState(0);

  useKeyboard((key) => {
    if (!isOpen) return;

    if (key.name === "escape") {
      onClose();
      return;
    }

    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, playlists.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "enter" || key.name === "return") {
      if (playlists[selectedIndex]) {
        onSelect(playlists[selectedIndex].id);
      }
    }
  });

  if (!isOpen) return null;

  return (
    <box
      position="absolute"
      width="100%"
      height="100%"
      justifyContent="center"
      alignItems="center"
    >
      <box
        width={40}
        backgroundColor="#282828"
        borderStyle="rounded"
        flexDirection="column"
        padding={1}
        gap={1}
      >
        <text fg="#ffffff"><b>Add to Playlist</b></text>
        <text fg="#b3b3b3">Select with j/k, Enter to confirm, Esc to cancel</text>
        
        <box flexDirection="column" marginTop={1}>
          {playlists.map((pl, i) => (
            <box key={pl.id} backgroundColor={selectedIndex === i ? "#3e3e3e" : undefined} paddingX={1}>
              <text fg={selectedIndex === i ? "#1DB954" : "#ffffff"}>{pl.name}</text>
            </box>
          ))}
          {playlists.length === 0 && (
            <text fg="#b3b3b3">No playlists available</text>
          )}
        </box>
      </box>
    </box>
  );
}
