import { useState } from "react";
import { useKeyboard } from "@opentui/react";

const PLAYLISTS = [
  { title: "Liked Songs", desc: "Playlist • 234 songs", color: "#7b68ee" },
  { title: "Daily Mix 1", desc: "Playlist • Spotify", color: "#1DB954" },
  { title: "Discover Weekly", desc: "Playlist • Spotify", color: "#ff6347" },
];

export function Sidebar({ isFocused }: { isFocused?: boolean }) {
  const [selectedIndex, setSelectedIndex] = useState(0);

  useKeyboard((key) => {
    if (!isFocused) return;
    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, PLAYLISTS.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    }
  });

  return (
    <box width={30} backgroundColor={isFocused ? "#181818" : "#121212"} flexDirection="column" paddingX={2} paddingTop={1}>
      <text fg="#b3b3b3"><b>Your Library</b></text>
      
      <box flexDirection="column" paddingTop={1} gap={1}>
        {PLAYLISTS.map((pl, i) => {
          const isSelected = isFocused && selectedIndex === i;
          return (
            <box key={i} flexDirection="row" gap={1} alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined}>
              <text fg={pl.color}>██</text>
              <box flexDirection="column">
                <text fg={isSelected ? "#1DB954" : "#ffffff"}>{pl.title}</text>
                <text fg="#b3b3b3">{pl.desc}</text>
              </box>
            </box>
          );
        })}
      </box>
    </box>
  );
}
