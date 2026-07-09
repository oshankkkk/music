export function Sidebar() {
  return (
    <box width={30} backgroundColor="#121212" flexDirection="column" paddingX={2} paddingTop={1}>
      <text fg="#b3b3b3"><b>Your Library</b></text>
      
      <box flexDirection="column" paddingTop={1} gap={1}>
        <box flexDirection="row" gap={1} alignItems="center">
          <text fg="#7b68ee">██</text>
          <box flexDirection="column">
            <text fg="#ffffff">Liked Songs</text>
            <text fg="#b3b3b3">Playlist • 234 songs</text>
          </box>
        </box>
        <box flexDirection="row" gap={1} alignItems="center">
          <text fg="#1DB954">██</text>
          <box flexDirection="column">
            <text fg="#ffffff">Daily Mix 1</text>
            <text fg="#b3b3b3">Playlist • Spotify</text>
          </box>
        </box>
        <box flexDirection="row" gap={1} alignItems="center">
          <text fg="#ff6347">██</text>
          <box flexDirection="column">
            <text fg="#ffffff">Discover Weekly</text>
            <text fg="#b3b3b3">Playlist • Spotify</text>
          </box>
        </box>
      </box>
    </box>
  );
}
