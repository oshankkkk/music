import terminalImage from 'terminal-image';
import { useEffect, useState } from 'react';
import React from 'react';

export function ImageRenderable({ src, width, height }: { src: string, width?: number | string, height?: number | string }) {
  if (!src) {
    return <box width={width as any} height={height as any} backgroundColor="#333" />;
  }

  return (
    <box width={width as any} height={height as any} backgroundColor="#282828" alignItems="center" justifyContent="center">
      <text fg="#1DB954">🎵</text>
    </box>
  );
}
