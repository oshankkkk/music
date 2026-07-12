export function ContextPanel() {
  return (
    <box width={35} backgroundColor="#121212" flexDirection="column" paddingX={2} paddingTop={1} gap={1}>
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
      
      <box flexDirection="column">
        <text fg="#ffffff"><b>Rick Astley</b></text>
        <text fg="#b3b3b3">Verified Artist ✓</text>
        <text fg="#b3b3b3">6,243,182 listeners</text>
      </box>
      
      <box borderStyle="single" paddingX={1} width={12} alignItems="center" justifyContent="center">
        <text fg="#ffffff">Follow</text>
      </box>
      
      <text fg="#b3b3b3">Rick Astley is a British singer, songwriter and radio personality...</text>
    </box>
  );
}
