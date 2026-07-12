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

export function Playbar({ isFocused, isPlaying, onTogglePlay }: { isFocused?: boolean; isPlaying: boolean; onTogglePlay?: () => void }) {
  const [song, setSong] = useState({
    title: "Never Gonna Give You Up",
    description: "Rick Astley's greatest hit",
    artist: "Rick Astley",
    timestamp: 97,
    duration: 212,
    albumArtUrl: "https://example.com/art.jpg",
    isLiked: true,
    playlists: [1, 2, 3],
    isPlayed: isPlaying
  });
  
  const [volume, setVolume] = useState(50);
  const [lyrics, setLyrics] = useState(false);

  const [playFlash, setPlayFlash] = useState(0);
  const [prevFlash, setPrevFlash] = useState(0);
  const [nextFlash, setNextFlash] = useState(0);
  const [volFlash, setVolFlash] = useState(0);

  const tPlay = useTimeline({ duration: 150, autoplay: false });
  const tPrev = useTimeline({ duration: 150, autoplay: false });
  const tNext = useTimeline({ duration: 150, autoplay: false });
  const tVol = useTimeline({ duration: 150, autoplay: false });

  useEffect(() => {
    tPlay.add({ v: 1 }, { v: 0, duration: 150, onUpdate: (a: any) => setPlayFlash(a.targets[0].v) });
    tPrev.add({ v: 1 }, { v: 0, duration: 150, onUpdate: (a: any) => setPrevFlash(a.targets[0].v) });
    tNext.add({ v: 1 }, { v: 0, duration: 150, onUpdate: (a: any) => setNextFlash(a.targets[0].v) });
    tVol.add({ v: 1 }, { v: 0, duration: 150, onUpdate: (a: any) => setVolFlash(a.targets[0].v) });
  }, [tPlay, tPrev, tNext, tVol]);

  const triggerFlash = (timeline: any) => {
    timeline.restart();
  };

  const handlePlayToggle = () => {
    triggerFlash(tPlay);
    setSong(s => ({ ...s, isPlayed: !s.isPlayed }));
    if (onTogglePlay) onTogglePlay();
  };

  const handleNext = () => {
    triggerFlash(tNext);
    setSong(s => ({ ...s, title: s.title === "Never Gonna Give You Up" ? "Together Forever" : "Never Gonna Give You Up", timestamp: 0 }));
  };

  const handlePrev = () => {
    triggerFlash(tPrev);
    setSong(s => ({ ...s, timestamp: 0 }));
  };

  // Flash the play icon whenever isPlaying changes (globally)
  const isFirstRender = useRef(true);
  useEffect(() => {
    if (isFirstRender.current) {
      isFirstRender.current = false;
      return;
    }
    triggerFlash(tPlay);
    setSong(s => ({ ...s, isPlayed: isPlaying }));
  }, [isPlaying]);

  useKeyboard((key) => {
    if (!isFocused) return;
    switch (key.name) {
      case "j":
        setVolume((v) => Math.max(0, v - 5));
        triggerFlash(tVol);
        break;
      case "k":
        setVolume((v) => Math.min(100, v + 5));
        triggerFlash(tVol);
        break;
      case "h":
        handlePrev();
        break;
      case "l":
        handleNext();
        break;
    }
  });

  const progressWidth = 27;
  const progressRatio = song.timestamp / song.duration;
  const progressFilled = Math.max(0, Math.min(progressWidth, Math.round(progressRatio * progressWidth)));
  const progressEmpty = Math.max(0, progressWidth - progressFilled);
  const progressStrFilled = "─".repeat(Math.max(0, progressFilled - 1)) + (progressFilled > 0 ? "●" : "");
  const progressStrEmpty = "─".repeat(progressEmpty);

  return (
    <box flexDirection="row" width="100%" height={4} backgroundColor="#181818" alignItems="center" paddingX={2}>
      <box flexDirection="row" width={30} alignItems="center" gap={1}>
        <text fg="#1DB954">██</text>
        <box flexDirection="column">
          <text fg="#ffffff">{song.title}</text>
          <text fg="#b3b3b3">{song.artist}</text>
        </box>
        <text 
          fg="#1DB954" 
          paddingLeft={1} 
          onMouseDown={() => setSong(s => ({ ...s, isLiked: !s.isLiked }))}
        >
          {song.isLiked ? '♥' : '♡'}
        </text>
      </box>

      <box flexDirection="column" flexGrow={1} alignItems="center" justifyContent="center">
        <box flexDirection="row" gap={2}>
          <text fg={interpolateGray(prevFlash)} onMouseDown={handlePrev}>{'|<'}</text>
          <text fg={interpolatePlay(playFlash)} onMouseDown={handlePlayToggle}>{song.isPlayed ? '[ || ]' : '[ > ]'}</text>
          <text fg={interpolateGray(nextFlash)} onMouseDown={handleNext}>{'>|'}</text>
        </box>
        <box flexDirection="row" gap={1} alignItems="center">
          <text fg="#b3b3b3">{formatTime(song.timestamp)}</text>
          <text fg="#1DB954">{progressStrFilled}</text>
          <text fg="#404040">{progressStrEmpty}</text>
          <text fg="#b3b3b3">{formatTime(song.duration)}</text>
        </box>
      </box>

      <box flexDirection="row" width={30} justifyContent="flex-end" alignItems="center" gap={1}>
        <text fg={setLyrics? "#1DB954" : "#b3b3b3"} onMouseDown={() => setLyrics(!lyrics)}>🎤</text>
        <text fg="#b3b3b3">♫</text>
        <text fg="#b3b3b3" onMouseDown={() => { setVolume(v => (v === 0 ? 50 : 0)); triggerFlash(tVol); }}>Vol</text>
        <text fg={interpolateVol(volFlash)}>{volume.toString().padStart(3, ' ')}%</text>
      </box>
    </box>
  );
}
