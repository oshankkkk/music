import terminalImage from 'terminal-image';
import { useEffect, useState } from 'react';
import React from 'react';

export function ImageRenderable({ src, width, height }: { src: string, width?: number | string, height?: number | string }) {
  const [ansi, setAnsi] = useState<string>("");

  useEffect(() => {
    async function fetchImage() {
      if (!src) {
        setAnsi("");
        return;
      }
      try {
        const response = await fetch(src);
        const buffer = await response.arrayBuffer();
        const str = await terminalImage.buffer(Buffer.from(buffer), { width, height });
        setAnsi(str);
      } catch (err) {
        console.error("Failed to load image", err);
        setAnsi("");
      }
    }
    fetchImage();
  }, [src, width, height]);

  if (!ansi) {
    return <box width={width as any} height={height as any} backgroundColor="#333" />;
  }

  return <text>{ansi}</text>;
}
