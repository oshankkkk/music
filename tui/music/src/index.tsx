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
          <text fg="#b3b3b3"><b>Your Library</b></text>
          
          <box flexDirection="column" paddingTop={1} gap={1}>
            {/* Library Item 1 */}
            <box flexDirection="row" gap={1} alignItems="center">
              <text fg="#7b68ee">██</text>
              <box flexDirection="column">
                <text fg="#ffffff">Liked Songs</text>
                <text fg="#b3b3b3">Playlist • 234 songs</text>
              </box>
            </box>
            {/* Library Item 2 */}
            <box flexDirection="row" gap={1} alignItems="center">
              <text fg="#1DB954">██</text>
              <box flexDirection="column">
                <text fg="#ffffff">Daily Mix 1</text>
                <text fg="#b3b3b3">Playlist • Spotify</text>
              </box>
            </box>
            {/* Library Item 3 */}
            <box flexDirection="row" gap={1} alignItems="center">
              <text fg="#ff6347">██</text>
              <box flexDirection="column">
                <text fg="#ffffff">Discover Weekly</text>
                <text fg="#b3b3b3">Playlist • Spotify</text>
              </box>
            </box>
          </box>
        </box>
        
        {/* Main Content */}
        <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
          <text fg="#ffffff"><b>Good evening</b></text>
          
          {/* Quick Access Grid */}
          <box flexDirection="column" paddingTop={1} gap={1}>
            <box flexDirection="row" gap={2}>
              {/* Card 1 */}
              <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
                <text fg="#7b68ee">████</text>
                <text fg="#ffffff">Liked Songs</text>
              </box>
              {/* Card 2 */}
              <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
                <text fg="#1DB954">████</text>
                <text fg="#ffffff">Daily Mix 1</text>
              </box>
            </box>
            <box flexDirection="row" gap={2}>
              {/* Card 3 */}
              <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
                <text fg="#ff6347">████</text>
                <text fg="#ffffff">Discover Weekly</text>
              </box>
              {/* Card 4 */}
              <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
                <text fg="#4169e1">████</text>
                <text fg="#ffffff">Viral Hits</text>
              </box>
            </box>
          </box>

          {/* Section: Made for you */}
          <box flexDirection="row" justifyContent="space-between" paddingTop={2} paddingBottom={1} width="100%">
            <text fg="#ffffff"><b>Made for you</b></text>
            <text fg="#b3b3b3">Show all</text>
          </box>
          
          <box flexDirection="row" gap={2}>
            {/* Mix Card 1 */}
            <box flexDirection="column" width={18} backgroundColor="#181818" padding={1} gap={1}>
              <text fg="#1DB954">
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████
              </text>
              <text fg="#ffffff"><b>Daily Mix 2</b></text>
              <text fg="#b3b3b3">Post Malone...</text>
            </box>
            {/* Mix Card 2 */}
            <box flexDirection="column" width={18} backgroundColor="#181818" padding={1} gap={1}>
              <text fg="#ff1493">
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████
              </text>
              <text fg="#ffffff"><b>Daily Mix 3</b></text>
              <text fg="#b3b3b3">Dua Lipa...</text>
            </box>
          </box>
        </box>
        
        {/* Right Context Panel */}
        <box width={35} backgroundColor="#121212" flexDirection="column" paddingX={2} paddingTop={1} gap={1}>
          {/* Cover Art */}
          <text fg="#1DB954">
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████{"\n"}
            ██████████████████████████████
          </text>
          
          {/* Info Card */}
          <box flexDirection="column">
            <text fg="#ffffff"><b>Rick Astley</b></text>
            <text fg="#b3b3b3">Verified Artist ✓</text>
            <text fg="#b3b3b3">6,243,182 listeners</text>
          </box>
          
          {/* Follow Button */}
          <box borderStyle="single" paddingX={1} width={12} alignItems="center" justifyContent="center">
            <text fg="#ffffff">Follow</text>
          </box>
          
          {/* Bio */}
          <text fg="#b3b3b3">Rick Astley is a British singer, songwriter and radio personality...</text>
        </box>
      </box>
      
      {/* Bottom Playbar */}
      <box flexDirection="row" width="100%" height={4} backgroundColor="#181818" alignItems="center" paddingX={2}>
        {/* Left: Track Info */}
        <box flexDirection="row" width={30} alignItems="center" gap={1}>
          <text fg="#1DB954">██</text>
          <box flexDirection="column">
            <text fg="#ffffff">Never Gonna Give You Up</text>
            <text fg="#b3b3b3">Rick Astley</text>
          </box>
          <text fg="#1DB954" paddingLeft={1}>♥</text>
        </box>

        {/* Center: Controls & Progress */}
        <box flexDirection="column" flexGrow={1} alignItems="center" justifyContent="center">
          <box flexDirection="row" gap={2}>
            <text fg="#b3b3b3">{'|<'}</text>
            <text fg="#ffffff">{'[ > ]'}</text>
            <text fg="#b3b3b3">{'>|'}</text>
          </box>
          <box flexDirection="row" gap={1} alignItems="center">
            <text fg="#b3b3b3">1:37</text>
            <text fg="#1DB954">────────●</text>
            <text fg="#404040">──────────────────</text>
            <text fg="#b3b3b3">3:32</text>
          </box>
        </box>

        {/* Right: Extra Controls */}
        <box flexDirection="row" width={30} justifyContent="flex-end" alignItems="center" gap={1}>
          <text fg="#b3b3b3">🎤</text>
          <text fg="#b3b3b3">♫</text>
          <text fg="#b3b3b3">Vol</text>
          <text fg="#ffffff">────●</text>
          <text fg="#404040">──────</text>
        </box>
      </box>
    </box>
  );
}

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);
