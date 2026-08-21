import { rpcCall } from "../client/client";
import { SongSearchBar } from "./SongSearchBar";

export function SongPlaylistAddSearch({ isOpen, playlistId }: { isOpen: boolean, playlistId?: number }) {
	const handleSubmit = async (query: string) => {
		try {
			rpcCall("lib-addsongtoplaylist", "lib", { songName: query, playlistId: playlistId });
		} catch (e) {
			console.error(e);
		}
	};

	return <SongSearchBar isOpen={isOpen} onSubmit={handleSubmit} isPlaylist={true} />;
}
