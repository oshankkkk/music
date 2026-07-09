import { useState, useRef, useCallback, useEffect } from "react";
import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";

const LEADER_TIMEOUT_MS = 300;

function App() {
  const renderer = useRenderer();
  const [focusArea, setFocusArea] = useState<"none" | "sidebar" | "quick-access" | "mixes">("none");
  const [isPlaying, setIsPlaying] = useState(false);

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
    setIsPlaying((p) => !p);
  }, [playToggleTick]);

  const togglePlay = useCallback(() => {
    setIsPlaying((p) => !p);
  }, []);

  // Safe deferred toggle: bumps a state counter so the actual toggle
  // happens inside a React useEffect, not a raw setTimeout
  const deferredTogglePlay = useCallback(() => {
    setPlayToggleTick((t) => t + 1);
  }, []);

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
      setFocusArea("none");
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
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar isFocused={focusArea === "sidebar"} />
        <MainContent focusArea={focusArea} />
        <ContextPanel />
      </box>
      <Playbar isFocused={focusArea === "none"} isPlaying={isPlaying} />
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);

