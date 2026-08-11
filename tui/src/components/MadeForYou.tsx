import { useState } from "react";
import { useKeyboard } from "@opentui/react";

const MIXES = [
  { title: "Daily Mix 2", subtitle: "Post Malone...", color: "#1DB954" },
  { title: "Daily Mix 3", subtitle: "Dua Lipa...", color: "#ff1493" },
];

export function MadeForYou({ isFocused }: { isFocused: boolean }) {
  const [mixIndex, setMixIndex] = useState(0);

  useKeyboard((key) => {
    if (isFocused) {
      if (key.name === "l") setMixIndex((i) => Math.min(i + 1, MIXES.length - 1));
      if (key.name === "h") setMixIndex((i) => Math.max(i - 1, 0));
    }
  });

  return (
    <box flexDirection="column" gap={0} width="100%">
      <box flexDirection="row" justifyContent="space-between" paddingTop={2} paddingBottom={1} width="100%">
        <box flexDirection="column" gap={0}>
          <text fg="#ffffff"><b>Made for you</b></text>
          <text fg="#888888">Select: space+m</text>
        </box>
        <text fg="#b3b3b3">Show all</text>
      </box>
      
      <box flexDirection="row" gap={2}>
        {MIXES.map((mix, i) => {
          const isSelected = isFocused && mixIndex === i;
          return (
            <box key={i} flexDirection="column" width={18} backgroundColor={isSelected ? "#282828" : "#181818"} padding={1} gap={1}>
              <text fg={mix.color}>
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████{"\n"}
                ██████████████
              </text>
              <text fg={isSelected ? "#1DB954" : "#ffffff"}><b>{mix.title}</b></text>
              <text fg="#b3b3b3">{mix.subtitle}</text>
            </box>
          );
        })}
      </box>
    </box>
  );
}
