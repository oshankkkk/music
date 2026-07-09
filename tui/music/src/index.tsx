import { useState, useRef, useCallback } from "react";
import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";

function App() {
  const renderer = useRenderer();
  const [focusArea, setFocusArea] = useState<"none" | "sidebar" | "quick-access" | "mixes">("none");
  const [isPlaying, setIsPlaying] = useState(false);
  const pendingSpaceRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  const togglePlay = useCallback(() => {
    setIsPlaying((p) => !p);
  }, []);

  useKeyboard((key) => {
    if (key.name === "escape" || (key.ctrl && key.name === "c")) {
      renderer.destroy();
      process.exit(0);
    }

    if (key.name === "escape") {
      if (pendingSpaceRef.current) {
        clearTimeout(pendingSpaceRef.current);
        pendingSpaceRef.current = null;
      }
      setFocusArea("none");
      return;
    }

    // If there's a pending space, check for sequence completion
    if (pendingSpaceRef.current !== null) {
      clearTimeout(pendingSpaceRef.current);
      pendingSpaceRef.current = null;

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

    // Buffer space for potential sequence
    if (key.name === "space") {
      pendingSpaceRef.current = setTimeout(() => {
        pendingSpaceRef.current = null;
        togglePlay();
      }, 300);
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

