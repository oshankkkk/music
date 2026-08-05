import { useState, useRef, useCallback, useEffect } from "react";
import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";
import { SearchPopup } from "./components/SearchPopup";

const LEADER_TIMEOUT_MS = 300;

function App() {
  const renderer = useRenderer();
  const [focusArea, setFocusArea] = useState<"none" | "sidebar" | "quick-access" | "mixes">("none");
  const [isSearchOpen, setIsSearchOpen] = useState(false);
  const [isPlaying, setIsPlaying] = useState(false);
  const [song, setSong] = useState({
    title: "Never Gonna Give You Up",
    description: "Rick Astley's greatest hit",
    artist: "Rick Astley",
    timestamp: 97,
    duration: 212,
    albumArtUrl: "https://example.com/art.jpg",
    isLiked: true,
    playlists: [1, 2, 3],
    isPlayed: false
  });

  // Track when space was pressed (timestamp), not a setTimeout handle
  const spaceTimestampRef = useRef<number | null>(null);

  // State-based trigger for deferred play toggles (safe for React/OpenTUI lifecycle)
  const [playToggleTick, setPlayToggleTick] = useState(0);
  const isFirstTickRef = useRef(true);

  // When playToggleTick changes (and it's not the initial mount), toggle play
  useEffect(() => {
    if (isFirstTickRef.current) {
      isFirstTickRef.current = false;
      return;
    }
    import("./client/client").then(({ rpcCall }) => {
      rpcCall("player-toggle-pause").catch(console.error);
    });
    setIsPlaying((p) => !p);
  }, [playToggleTick]);

  // Safe deferred toggle: bumps a state counter so the actual toggle
  // happens inside a React useEffect, not a raw setTimeout
  const deferredTogglePlay = useCallback(() => {
    setPlayToggleTick((t) => t + 1);
  }, []);

  const togglePlay = useCallback(() => {
    deferredTogglePlay();
  }, [deferredTogglePlay]);

  // Poll for expired pending space presses.
  // This replaces the setTimeout-based approach that crashed when
  // togglePlay was called outside the renderer's event processing cycle.
  useEffect(() => {
    const intervalId = setInterval(() => {
      if (
        spaceTimestampRef.current !== null &&
        Date.now() - spaceTimestampRef.current >= LEADER_TIMEOUT_MS
      ) {
        spaceTimestampRef.current = null;
        deferredTogglePlay();
      }
    }, 50);
    return () => clearInterval(intervalId);
  }, [deferredTogglePlay]);

  useKeyboard((key) => {
    // Ctrl+C exits the app
    if (key.ctrl && key.name === "c") {
      renderer.destroy();
      process.exit(0);
    }

    // Escape unfocuses (resets to playbar) and clears pending leader
    if (key.name === "escape") {
      spaceTimestampRef.current = null;
      if (isSearchOpen) {
        setIsSearchOpen(false);
      } else {
        setFocusArea("none");
      }
      return;
    }

    if (isSearchOpen) {
      return;
    }

    // If there's a pending space, check for sequence completion
    if (spaceTimestampRef.current !== null) {
      spaceTimestampRef.current = null;

      switch (key.name) {
        case "p": setFocusArea("sidebar"); return;
        case "n": setFocusArea("quick-access"); return;
        case "m": setFocusArea("mixes"); return;
        case "b": setFocusArea("none"); return;
        case "s": setIsSearchOpen(true); return;
        default:
          // Not a sequence key — fire the buffered space as play/pause
          togglePlay();
          break; // fall through to handle current key normally
      }
    }

    // Buffer space for potential leader sequence
    if (key.name === "space") {
      spaceTimestampRef.current = Date.now();
      return;
    }
  });

  return (
    <box flexDirection="column" width="100%" height="100%" backgroundColor="#000000">
      <box width="100%" height={1} justifyContent="center" alignItems="center" backgroundColor="#181818">
        <text fg="#b3b3b3">Search press space+s</text>
      </box>
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar isFocused={focusArea === "sidebar" && !isSearchOpen} />
        <MainContent focusArea={isSearchOpen ? "none" : focusArea} />
        <ContextPanel />
      </box>
      <Playbar isFocused={focusArea === "none" && !isSearchOpen} isPlaying={isPlaying} onTogglePlay={togglePlay} song={song} setSong={setSong} />
      <SearchPopup isOpen={isSearchOpen} onClose={() => setIsSearchOpen(false)} onSongSelect={(newSong) => {
        setSong(s => ({
          ...s, 
          title: newSong.title || s.title,
          artist: newSong.artist || s.artist,
          duration: Number(newSong.duration) || s.duration,
          timestamp: 0,
          isPlayed: true
        }));
        if (!isPlaying) setIsPlaying(true);
      }} />
    </box>
  );
}

import { startReader } from "./client/client";

const renderer = await createCliRenderer();
startReader();
createRoot(renderer).render(<App />);
