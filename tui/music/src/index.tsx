import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";
import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";

function App() {
  const renderer = useRenderer();

  useKeyboard((key) => {
    if (key.name === "escape" || (key.ctrl && key.name === "c")) {
      renderer.destroy();
      process.exit(0);
    }
  });

  return (
    <box flexDirection="column" width="100%" height="100%" backgroundColor="#000000">
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar />
        <MainContent />
        <ContextPanel />
      </box>
      <Playbar />
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);
