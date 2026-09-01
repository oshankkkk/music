import { useState } from "react";

interface ContextPanelProps {
  artistName: string;
  artistDescription: string;
}

export function ContextPanel({ artistName, artistDescription }: ContextPanelProps) {
  const [artist] = useState({
    id: "rick_astley_01",
    monthlyListeners: "",
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
        <text fg="#ffffff"><b>{artistName}</b></text>
		{/**<text fg="#b3b3b3">{artist.monthlyListeners} listeners</text>**/}
      </box>
      
      <text fg="#b3b3b3">{artistDescription}</text>
    </box>
  );
}
