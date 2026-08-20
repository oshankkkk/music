import { useState, useEffect } from "react";
import type { PlaylistInfo } from "../client/types";

export function RenamePlaylistPopup({ playlist, onSubmit, onClose }: { playlist: PlaylistInfo | null, onSubmit: (name: string) => void, onClose: () => void }) {
	const [name, setName] = useState("");

    useEffect(() => {
        if (playlist) {
            setName(playlist.name);
        }
    }, [playlist]);

	const handleSubmit = () => {
        if (name.trim()) {
		    onSubmit(name);
        }
        onClose();
        setName("");
	};

	if (!playlist) return null;

	return (
		<box
		position="absolute"
		width="100%"
		height="100%"
		justifyContent="center"
		alignItems="center"
		>
		<box
		width={50}
		height={5}
		backgroundColor="#282828"
		borderStyle="rounded"
		flexDirection="column"
		paddingX={1}
		>
		<text fg="#1DB954"><b>Rename Playlist</b></text>
		<input
		placeholder="Enter new playlist name..."
        value={name}
		onInput={setName}
		onSubmit={handleSubmit}
		focused={true}
		/>
		</box>
		</box>
	);
}
