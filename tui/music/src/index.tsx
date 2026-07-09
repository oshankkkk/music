import { createCliRenderer } from "@opentui/core";
import { createRoot, useKeyboard, useRenderer } from "@opentui/react";

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
      {/* Top Section */}
      <box flexDirection="row" width="100%" flexGrow={1}>
        {/* Left Sidebar */}
        <box width={30} backgroundColor="#121212" flexDirection="column" paddingX={2} paddingTop={1}>
          <text fg="#b3b3b3">Your Library</text>
        </box>
        
        {/* Main Content */}
        <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
          <text fg="#ffffff">Good evening</text>
        </box>
        
        {/* Right Context Panel */}
        <box width={35} backgroundColor="#121212" flexDirection="column" paddingX={2} paddingTop={1}>
          <text fg="#b3b3b3">Context Panel</text>
        </box>
      </box>
      
      {/* Bottom Playbar */}
      <box flexDirection="row" width="100%" height={4} backgroundColor="#181818" alignItems="center" paddingX={3}>
        <text fg="#ffffff">Playbar</text>
      </box>
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);
