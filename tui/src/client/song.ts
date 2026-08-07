export type Song = {
  title: string;
  description: string;
  artist: string;
  timestamp: number;
  duration: number;
  albumArtUrl: string;
  isLiked: boolean;
  playlists: number[];
  isPlayed: boolean;
};

