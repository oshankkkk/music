import { useEffect, useState } from 'react';
import React from 'react';

export function ImageRenderable({ src, width, height }: { src?: string, width?: number | string, height?: number | string }) {
  const [failed, setFailed] = useState(false);

  // Reset the failure state whenever we get a new source (e.g. new song).
  useEffect(() => {
    setFailed(false);
  }, [src]);

  if (!src || failed) {
    return (
      <box width={width as any} height={height as any} backgroundColor="#282828" alignItems="center" justifyContent="center">
        <text fg="#1DB954">🎵</text>
      </box>
    );
  }

  return (
    <image
      source={src}
      fit="cover"
      protocol="auto"
      style={{ width: width as any, height: height as any }}
      onError={() => setFailed(true)}
    />
  );
}
