import { useState } from "react";

export function ContextPanel() {
  const [artist] = useState({
    id: "rick_astley_01",
    name: "Rick Astley",
    monthlyListeners: 6243182,
    description: "Rick Astley is a British singer, songwriter and radio personality...",
    socials: {
      twitter: "https://twitter.com/rickastley",
      instagram: "https://instagram.com/rickastley",
      youtube: "https://youtube.com/rickastley"
    },
    imageUrl: "https://example.com/rick.jpg"
  });

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
        <text fg="#ffffff"><b>{artist.name}</b></text>
        <text fg="#b3b3b3">{artist.monthlyListeners.toLocaleString()} listeners</text>
      </box>
      
      <text fg="#b3b3b3">{artist.description}</text>
    </box>
  );
}
