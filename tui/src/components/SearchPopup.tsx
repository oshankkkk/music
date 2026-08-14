import { useState } from "react";
import { rpcCall } from "../client/client";

export function SearchPopup({isOpen}: {isOpen: boolean}) {
	const [query, setQuery] = useState("");

	const handleSubmit = async (query: string) => {
		try {
			rpcCall("playSong", "song",{ songName: query });
			} catch (e) {
			console.error(e);
		}
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
		<text fg="#1DB954"><b>Search</b></text>
		<input
		placeholder="Type here..."
		onInput={setQuery}
		onSubmit={()=>{handleSubmit(query)}}
		focused={true}
		/>
		</box>
		</box>
	);
}


