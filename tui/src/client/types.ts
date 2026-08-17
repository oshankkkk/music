export type PlaylistInfo = {
  id: string;
  name: string;
};

export type Song = {
  id: string;
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

export type QueueItem = {
  queueId: string;
  songId: string;
  name: string;
};

export type RecentlyPlayedItem = {
  songId: string;
  name: string;
  isLiked: boolean;
};
