import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";
import type { QueueItem } from "../client/types";

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
            const newQueue = queue.filter((_, i) => i !== selectedIndex);
            setQueue(newQueue);
            if (selectedIndex >= newQueue.length) {
                setSelectedIndex(Math.max(0, newQueue.length - 1));
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
      
      <box flexDirection="column" marginTop={1} gap={1} flexGrow={1}>
        {queue.map((song, i) => {
          const isSelected = selectedIndex === i;
          return (
            <box key={song.queueId} flexDirection="row" gap={2} alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
              <text fg="#1DB954">██</text>
              <box flexDirection="column">
                <text fg={isSelected ? "#1DB954" : "#ffffff"}>{song.name}</text>
                <text fg="#b3b3b3">Song ID: {song.songId}</text>
              </box>
            </box>
          );
        })}
      </box>
    </box>
  );
}
