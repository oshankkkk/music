export function MainContent() {
  return (
    <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
      <text fg="#ffffff"><b>Good evening</b></text>
      
      <box flexDirection="column" paddingTop={1} gap={1}>
        <box flexDirection="row" gap={2}>
          <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
            <text fg="#7b68ee">████</text>
            <text fg="#ffffff">Liked Songs</text>
          </box>
          <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
            <text fg="#1DB954">████</text>
            <text fg="#ffffff">Daily Mix 1</text>
          </box>
        </box>
        <box flexDirection="row" gap={2}>
          <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
            <text fg="#ff6347">████</text>
            <text fg="#ffffff">Discover Weekly</text>
          </box>
          <box flexDirection="row" width={25} backgroundColor="#282828" alignItems="center" gap={1}>
            <text fg="#4169e1">████</text>
            <text fg="#ffffff">Viral Hits</text>
          </box>
        </box>
      </box>

      <box flexDirection="row" justifyContent="space-between" paddingTop={2} paddingBottom={1} width="100%">
        <text fg="#ffffff"><b>Made for you</b></text>
        <text fg="#b3b3b3">Show all</text>
      </box>
      
      <box flexDirection="row" gap={2}>
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
  );
}
