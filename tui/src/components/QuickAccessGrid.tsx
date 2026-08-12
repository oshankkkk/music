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

export function QuickAccessGrid({ isFocused }: { isFocused: boolean }) {
  const [qaRow, setQaRow] = useState(0);
  const [qaCol, setQaCol] = useState(0);

  useKeyboard((key) => {
    if (isFocused) {
      if (key.name === "j") setQaRow((r) => Math.min(r + 1, QUICK_ACCESS.length - 1));
      if (key.name === "k") setQaRow((r) => Math.max(r - 1, 0));
      if (key.name === "l") setQaCol((c) => Math.min(c + 1, (QUICK_ACCESS[qaRow]?.length || 1) - 1));
      if (key.name === "h") setQaCol((c) => Math.max(c - 1, 0));
    }
  });

  return (
    <box flexDirection="column" gap={0}>
      <box flexDirection="column" gap={0}>
        <text fg="#ffffff"><b>Good evening</b></text>
        <text fg="#888888">Select: space+n</text>
      </box>
      
      <box flexDirection="column" paddingTop={1} gap={1}>
        {QUICK_ACCESS.map((row, ri) => (
          <box key={ri} flexDirection="row" gap={2}>
            {row.map((card, ci) => {
              const isSelected = isFocused && qaRow === ri && qaCol === ci;
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
    </box>
  );
}
