import { useState, useEffect, useRef } from "react";
import { useKeyboard, useTimeline } from "@opentui/react";

function interpolateGray(v: number) {
  const val = Math.round(179 + (255 - 179) * v);
  const hex = val.toString(16).padStart(2, '0');
  return `#${hex}${hex}${hex}`;
}

function interpolatePlay(v: number) {
  const r = Math.round(255 + (29 - 255) * v);
  const g = Math.round(255 + (185 - 255) * v);
  const b = Math.round(255 + (84 - 255) * v);
  return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`;
}

function interpolateVol(v: number) {
  const r = Math.round(29 + (255 - 29) * v);
  const g = Math.round(185 + (255 - 185) * v);
  const b = Math.round(84 + (255 - 84) * v);
  return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`;
}

function formatTime(seconds: number) {
  const m = Math.floor(seconds / 60);
  const s = Math.floor(seconds % 60);
  return `${m}:${s.toString().padStart(2, '0')}`;
}

export function Playbar({ isFocused, isPlaying }: { isFocused?: boolean; isPlaying: boolean }) {
  const [currentTime, setCurrentTime] = useState(97);
  const [duration, setDuration] = useState(212);
  const [volume, setVolume] = useState(50);
  const [isLiked, setIsLiked] = useState(true);
  const [currentTrack, setCurrentTrack] = useState({ title: "Never Gonna Give You Up", artist: "Rick Astley" });

  const [playFlash, setPlayFlash] = useState(0);
  const [prevFlash, setPrevFlash] = useState(0);
  const [nextFlash, setNextFlash] = useState(0);
  const [volFlash, setVolFlash] = useState(0);

  const tPlay = useTimeline({ duration: 150, autoplay: false });
  const tPrev = useTimeline({ duration: 150, autoplay: false });
  const tNext = useTimeline({ duration: 150, autoplay: false });
  const tVol = useTimeline({ duration: 150, autoplay: false });

  const triggerFlash = (timeline: any, setter: (val: number) => void) => {
    timeline.clear();
    timeline.add({ v: 1 }, { v: 0, duration: 150, onUpdate: (a: any) => setter(a.targets[0].v) });
  };

  // Flash the play icon whenever isPlaying changes (globally)
  const isFirstRender = useRef(true);
  useEffect(() => {
    if (isFirstRender.current) {
      isFirstRender.current = false;
      return;
    }
    triggerFlash(tPlay, setPlayFlash);
  }, [isPlaying]);

  useKeyboard((key) => {
    if (!isFocused) return;
    switch (key.name) {
      case "j":
        setVolume((v) => Math.max(0, v - 5));
        triggerFlash(tVol, setVolFlash);
        break;
      case "k":
        setVolume((v) => Math.min(100, v + 5));
        triggerFlash(tVol, setVolFlash);
        break;
      case "h":
        triggerFlash(tPrev, setPrevFlash);
        break;
      case "l":
        triggerFlash(tNext, setNextFlash);
        break;
    }
  });

  const progressWidth = 27;
  const progressRatio = currentTime / duration;
  const progressFilled = Math.max(0, Math.min(progressWidth, Math.round(progressRatio * progressWidth)));
  const progressEmpty = Math.max(0, progressWidth - progressFilled);
  const progressStrFilled = "─".repeat(Math.max(0, progressFilled - 1)) + (progressFilled > 0 ? "●" : "");
  const progressStrEmpty = "─".repeat(progressEmpty);

  return (
    <box flexDirection="row" width="100%" height={4} backgroundColor="#181818" alignItems="center" paddingX={2}>
      <box flexDirection="row" width={30} alignItems="center" gap={1}>
        <text fg="#1DB954">██</text>
        <box flexDirection="column">
          <text fg="#ffffff">{currentTrack.title}</text>
          <text fg="#b3b3b3">{currentTrack.artist}</text>
        </box>
        <text fg="#1DB954" paddingLeft={1}>{isLiked ? '♥' : '♡'}</text>
      </box>

      <box flexDirection="column" flexGrow={1} alignItems="center" justifyContent="center">
        <box flexDirection="row" gap={2}>
          <text fg={interpolateGray(prevFlash)}>{'|<'}</text>
          <text fg={interpolatePlay(playFlash)}>{isPlaying ? '[ || ]' : '[ > ]'}</text>
          <text fg={interpolateGray(nextFlash)}>{'>|'}</text>
        </box>
        <box flexDirection="row" gap={1} alignItems="center">
          <text fg="#b3b3b3">{formatTime(currentTime)}</text>
          <text fg="#1DB954">{progressStrFilled}</text>
          <text fg="#404040">{progressStrEmpty}</text>
          <text fg="#b3b3b3">{formatTime(duration)}</text>
        </box>
      </box>

      <box flexDirection="row" width={30} justifyContent="flex-end" alignItems="center" gap={1}>
        <text fg="#b3b3b3">🎤</text>
        <text fg="#b3b3b3">♫</text>
        <text fg="#b3b3b3">Vol</text>
        <text fg={interpolateVol(volFlash)}>{volume.toString().padStart(3, ' ')}%</text>
      </box>
    </box>
  );
}
