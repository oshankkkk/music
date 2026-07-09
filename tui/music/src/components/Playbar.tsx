export function Playbar() {
  return (
    <box flexDirection="row" width="100%" height={4} backgroundColor="#181818" alignItems="center" paddingX={2}>
      <box flexDirection="row" width={30} alignItems="center" gap={1}>
        <text fg="#1DB954">██</text>
        <box flexDirection="column">
          <text fg="#ffffff">Never Gonna Give You Up</text>
          <text fg="#b3b3b3">Rick Astley</text>
        </box>
        <text fg="#1DB954" paddingLeft={1}>♥</text>
      </box>

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

      <box flexDirection="row" width={30} justifyContent="flex-end" alignItems="center" gap={1}>
        <text fg="#b3b3b3">🎤</text>
        <text fg="#b3b3b3">♫</text>
        <text fg="#b3b3b3">Vol</text>
        <text fg="#ffffff">────●</text>
        <text fg="#404040">──────</text>
      </box>
    </box>
  );
}
