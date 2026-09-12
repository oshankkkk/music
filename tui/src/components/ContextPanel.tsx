import { useState } from "react";
import { ImageRenderable } from "./Image";

interface ContextPanelProps {
  artistName: string;
  artistDescription: string;
  thumbnail?: string;
}

export function ContextPanel({ artistName, artistDescription, thumbnail }: ContextPanelProps) {
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
      <ImageRenderable src={thumbnail || ""} width={30} height={8} />
      
      <box flexDirection="column">
        <text fg="#ffffff"><b>{artistName}</b></text>
		{/**<text fg="#b3b3b3">{artist.monthlyListeners} listeners</text>**/}
      </box>
      
      <text fg="#b3b3b3">{artistDescription}</text>
    </box>
  );
}
