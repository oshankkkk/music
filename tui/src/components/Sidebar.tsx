import { useState, useRef } from "react";
import { useKeyboard } from "@opentui/react";
import type { PlaylistInfo } from "../client/types";

export function Sidebar({ isFocused, playlists, setPlaylists, onSelectPlaylist, onDeletePlaylist, onRenamePlaylist }: { isFocused?: boolean, playlists: PlaylistInfo[], setPlaylists: (p: PlaylistInfo[]) => void, onSelectPlaylist: (p: PlaylistInfo) => void, onDeletePlaylist: (id: string) => void, onRenamePlaylist: (p: PlaylistInfo) => void }) {
  const [selectedIndex, setSelectedIndex] = useState(0);
  const lastDPress = useRef<number>(0);

  useKeyboard((key) => {
    if (!isFocused) return;
    if (key.name === "j") {
      setSelectedIndex((prev) => Math.min(prev + 1, playlists.length - 1));
    } else if (key.name === "k") {
      setSelectedIndex((prev) => Math.max(prev - 1, 0));
    } else if (key.name === "enter" || key.name === "return") {
      if (playlists[selectedIndex]) {
        onSelectPlaylist(playlists[selectedIndex]);
      }
    } else if (key.name === "r") {
      if (playlists.length > 0 && playlists[selectedIndex]) {
        onRenamePlaylist(playlists[selectedIndex]);
      }
    } else if (key.name === "d") {
      const now = Date.now();
      if (now - lastDPress.current < 500) {
        if (playlists.length > 0 && playlists[selectedIndex]) {
          onDeletePlaylist(playlists[selectedIndex].id);
          // Don't modify the state locally, rely on the server refresh
        }
        lastDPress.current = 0;
      } else {
        lastDPress.current = now;
      }
    }
  });

  return (
    <box width={30} backgroundColor={isFocused ? "#181818" : "#121212"} flexDirection="column" paddingX={2} paddingTop={1}>
      <box flexDirection="row" justifyContent="space-between" width="100%">
        <box flexDirection="column" gap={0}>
          <text fg="#b3b3b3"><b>Your Library</b></text>
          <text fg="#888888">Select: space+p</text>
        </box>
        <text fg="#b3b3b3"><b>+</b></text>
      </box>
      
      <box flexDirection="column" paddingTop={1} gap={1}>
        {playlists.map((pl, i) => {
          const isSelected = isFocused && selectedIndex === i;
          return (
            <box key={pl.id} flexDirection="row" gap={1} alignItems="center" paddingX={1} backgroundColor={isSelected ? "#282828" : undefined} onMouseDown={() => { setSelectedIndex(i); onSelectPlaylist(pl); }}>
              <text fg="#1DB954">██</text>
              <box flexDirection="column">
                <text fg={isSelected ? "#1DB954" : "#ffffff"}>{pl.name}</text>
                <text fg="#b3b3b3">Playlist</text>
              </box>
            </box>
          );
        })}
      </box>
    </box>
  );
}
