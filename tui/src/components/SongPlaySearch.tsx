import { rpcCall } from "../client/client";
import { SongSearchBar } from "./SongSearchBar";

export function SongPlaySearch({ isOpen }: { isOpen: boolean }) {
	const handleSubmit = async (query: string) => {
		try {
			rpcCall("song-playSong", "song", { songName: query });
		} catch (e) {
			console.error(e);
		}
	};

	return <SongSearchBar isOpen={isOpen} onSubmit={handleSubmit} isPlaylist={false} />;
}
