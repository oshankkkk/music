import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";
import type { RecentlyPlayedItem } from "../client/types";

export function RecentlyPlayed({ isFocused, recentlyPlayed }: { isFocused: boolean, recentlyPlayed: RecentlyPlayedItem[] }) {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isFocused) return;
    if (key.name === "j") setSelectedIndex((prev) => Math.min(prev + 1, Math.max(0, recentlyPlayed.length - 1)));
    if (key.name === "k") setSelectedIndex((prev) => Math.max(prev - 1, 0));
  });

  return (
    <box flexDirection="column" width="100%">
      <box flexDirection="column" gap={0} paddingBottom={1}>
        <text fg="#ffffff"><b>Recently Played</b></text>
      </box>
      
      <box flexDirection="row" width="100%" paddingBottom={1} paddingX={1}>
        <text fg="#b3b3b3" width={4}>#</text>
        <text fg="#b3b3b3" flexGrow={1}>Title</text>
        <text fg="#b3b3b3" width={20}>Liked</text>
      </box>
      
      <box flexDirection="column" flexGrow={1} gap={0}>
        {recentlyPlayed.map((song, i) => {
          const isSelected = isFocused && selectedIndex === i;
          return (
            <box key={song.songId} flexDirection="row" width="100%" alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
              <text fg={isSelected ? "#1DB954" : "#b3b3b3"} width={4}>{String(i + 1)}</text>
              <box flexDirection="row" gap={1} flexGrow={1} alignItems="center">
                <text fg="#1DB954">██</text>
                <box flexDirection="column">
                  <text fg={isSelected ? "#1DB954" : "#ffffff"}>{song.name}</text>
                  <text fg="#b3b3b3">{song.artist}</text>
                </box>
              </box>
              <text fg="#b3b3b3" width={20}>{song.isLiked ? "♥" : "♡"}</text>
            </box>
          );
        })}
      </box>
    </box>
  );
}
