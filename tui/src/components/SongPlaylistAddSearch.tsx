import { rpcCall } from "../client/client";
import { SongSearchBar } from "./SongSearchBar";

export function SongPlaylistAddSearch({ isOpen }: { isOpen: boolean }) {
	const handleSubmit = async (query: string) => {
		try {
			rpcCall("addSongToPlaylist", "song", { songName: query });
		} catch (e) {
			console.error(e);
		}
	};

	return <SongSearchBar isOpen={isOpen} onSubmit={handleSubmit} />;
}
