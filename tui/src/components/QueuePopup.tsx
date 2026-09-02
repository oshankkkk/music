import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";
import type { QueueItem } from "../client/types";
import { rpcCall } from "../client/client";
import { SongList } from "./SongList";

export function QueuePopup({ isOpen, onClose, queue, setQueue }: { isOpen: boolean, onClose: () => void, queue: QueueItem[], setQueue: (q: QueueItem[]) => void }) {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isOpen) return;

    if (key.name === "escape") {
      onClose();
      return;
    }

    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, Math.max(0, queue.length - 1)));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "d") {
      const now = Date.now();
      if (now - lastDPress.current < 500) {
        // Double D pressed! Remove the selected song
        if (queue.length > 0) {
            rpcCall("lib-removefromqueue", "queue", { songindex: selectedIndex });
            if (selectedIndex >= queue.length - 1) {
                setSelectedIndex(Math.max(0, queue.length - 2));
            }
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
      
      <SongList
        items={queue}
        selectedIndex={selectedIndex}
        isFocused={isOpen}
        gap={1}
        marginTop={1}
        renderItem={(song, i, isSelected) => (
          <box flexDirection="row" gap={2} alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
            <text fg="#1DB954">██</text>
            <box flexDirection="column">
              <text fg={isSelected ? "#1DB954" : "#ffffff"}>{song.name}</text>
              <text fg="#b3b3b3">Song ID: {song.songId}</text>
            </box>
          </box>
        )}
      />
    </box>
  );
}
