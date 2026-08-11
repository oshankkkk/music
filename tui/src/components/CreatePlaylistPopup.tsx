import { useState } from "react";

export function CreatePlaylistPopup({ isOpen, onSubmit, onClose }: { isOpen: boolean, onSubmit: (name: string) => void, onClose: () => void }) {
	const [name, setName] = useState("");

	const handleSubmit = () => {
        if (name.trim()) {
		    onSubmit(name);
        }
        onClose();
        setName("");
	};

	if (!isOpen) return null;

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
		<text fg="#1DB954"><b>Create New Playlist</b></text>
		<input
		placeholder="Enter new playlist name..."
		onInput={setName}
		onSubmit={handleSubmit}
		focused={true}
		/>
		</box>
		</box>
	);
}
