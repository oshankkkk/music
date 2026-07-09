import { useState } from "react";
import { useKeyboard } from "@opentui/react";

const QUICK_ACCESS = [
  [
    { title: "Liked Songs", color: "#7b68ee" },
    { title: "Daily Mix 1", color: "#1DB954" },
  ],
  [
    { title: "Discover Weekly", color: "#ff6347" },
    { title: "Viral Hits", color: "#4169e1" },
  ],
];

const MIXES = [
  { title: "Daily Mix 2", subtitle: "Post Malone...", color: "#1DB954" },
  { title: "Daily Mix 3", subtitle: "Dua Lipa...", color: "#ff1493" },
];

export function MainContent({ focusArea }: { focusArea: string }) {
  const [qaRow, setQaRow] = useState(0);
  const [qaCol, setQaCol] = useState(0);
  const [mixIndex, setMixIndex] = useState(0);

  const isQuickAccessFocused = focusArea === "quick-access";
  const isMixesFocused = focusArea === "mixes";

  useKeyboard((key) => {
    if (isQuickAccessFocused) {
      if (key.name === "j") setQaRow((r) => Math.min(r + 1, QUICK_ACCESS.length - 1));
      if (key.name === "k") setQaRow((r) => Math.max(r - 1, 0));
      if (key.name === "l") setQaCol((c) => Math.min(c + 1, QUICK_ACCESS[qaRow].length - 1));
      if (key.name === "h") setQaCol((c) => Math.max(c - 1, 0));
    }
    if (isMixesFocused) {
      if (key.name === "l") setMixIndex((i) => Math.min(i + 1, MIXES.length - 1));
      if (key.name === "h") setMixIndex((i) => Math.max(i - 1, 0));
    }
  });

  return (
    <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
      <text fg="#ffffff"><b>Good evening</b></text>
      
      {/* Quick Access Grid */}
      <box flexDirection="column" paddingTop={1} gap={1}>
        {QUICK_ACCESS.map((row, ri) => (
          <box key={ri} flexDirection="row" gap={2}>
            {row.map((card, ci) => {
              const isSelected = isQuickAccessFocused && qaRow === ri && qaCol === ci;
              return (
                <box key={ci} flexDirection="row" width={25} backgroundColor={isSelected ? "#3e3e3e" : "#282828"} alignItems="center" gap={1}>
                  <text fg={card.color}>████</text>
                  <text fg={isSelected ? "#1DB954" : "#ffffff"}>{card.title}</text>
                </box>
              );
            })}
          </box>
        ))}
      </box>

      {/* Section: Made for you */}
      <box flexDirection="row" justifyContent="space-between" paddingTop={2} paddingBottom={1} width="100%">
        <text fg="#ffffff"><b>Made for you</b></text>
        <text fg="#b3b3b3">Show all</text>
      </box>
      
      <box flexDirection="row" gap={2}>
        {MIXES.map((mix, i) => {
          const isSelected = isMixesFocused && mixIndex === i;
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
