import { Song } from "./song.ts";

export function parseToSong(
  setSong: React.Dispatch<React.SetStateAction<Song>>,
  messageBuffer: Buffer,
) {
  try {
    const data = JSON.parse(messageBuffer.toString());
    if (data.result) {
      setSong((prevSong) => ({
        ...prevSong,
        title: data.result.title || prevSong.title,
        artist: data.result.artist || prevSong.artist,
        duration: data.result.duration || prevSong.duration,
        isLiked: data.result.isliked !== undefined ? data.result.isliked : prevSong.isLiked,
      }));
      console.log("set song wuna");
      console.log(data.result.title);
    }
  } catch (e) {
    console.error("Failed to parse JSON-RPC response in parseToSong", e);
  }
}
