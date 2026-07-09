import { useState } from "react";
import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";

function App() {
  const renderer = useRenderer();
  const [isSidebarFocused, setIsSidebarFocused] = useState(false);
  const [lastSeq, setLastSeq] = useState("");

  useKeyboard((key) => {
    if (key.name === "escape" || (key.ctrl && key.name === "c")) {
      renderer.destroy();
      process.exit(0);
    }
    
    setLastSeq(prev => {
      const seq = `${prev} ${key.name}`.trim();
      if (seq.endsWith("space p")) {
        setIsSidebarFocused(true);
        return "";
      }
      const parts = seq.split(" ");
      return parts.slice(-2).join(" ");
    });

    if (key.name === "escape") {
      setIsSidebarFocused(false);
    }
  });

  return (
    <box flexDirection="column" width="100%" height="100%" backgroundColor="#000000">
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar isFocused={isSidebarFocused} />
        <MainContent />
        <ContextPanel />
      </box>
      <Playbar isSidebarFocused={isSidebarFocused} />
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);
