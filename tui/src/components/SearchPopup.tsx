import { useState } from "react";

export function SearchPopup({ isOpen, onClose, onSongSelect }: { isOpen: boolean; onClose: () => void; onSongSelect?: (song: any) => void }) {
	const [query, setQuery] = useState("");

	const handleSubmit = async (query: string) => {
		console.log("hu");
		let message = {
			jsonrpc: "2.0",
			method: "playSong",		
			params: {
				songName: query
			},
			id: 1
		};

		let request = JSON.stringify(message);		
		let buff = Buffer.alloc(0);
		const socket = await Bun.connect({
			unix: "../build/us.socket",
			socket: {
				open(socket) {
					console.log("msg send");
					socket.write(request);
				},
				data(socket, data) {
					buff = Buffer.concat([buff, data]);
					while (true) {
						if (buff.length < 4) break;
						console.log("buff legnth",buff.length);
						let msgLen = buff.readUInt32LE(0);
						if (buff.length < 4 + msgLen){
							console.log("awul");
							break;
						} 
						let rpcmsg = buff.subarray(4, 4 + msgLen);
							console.log("rpclist",rpcmsg.toString());
						try {
							const response = JSON.parse(rpcmsg.toString());
							console.log("res",response)
							if (response.result && onSongSelect) {

							console.log("resresult",response.result)
								onSongSelect(response.result);
								onClose();
							}
						} catch (e) {
							console.error(e);
						}
						
						buff = buff.subarray(4 + msgLen);
					}
				},
				close(socket) {
					console.log("Disconnected");
				},
				error(socket, error) {
					console.error(error);
				}
			}
		});
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


