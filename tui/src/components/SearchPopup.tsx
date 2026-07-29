import { useState } from "react";

async function handleSubmit(query:string){
		console.log("hu");
	let message={
		jsonrpc:"2.0",
		method:"playSong",		
		params:{
			songName:query
		},
		id:1
	}	
	let request=JSON.stringify(message)		
	const socket=await Bun.connect({
		unix:"../build/us.socket",
		socket:{
			open(socket){
				console.log("msg send");
				socket.write(request)
			},
			data(socket,data){
				readResponse(data)	
			},

			close(socket) {
				console.log("Disconnected");
			},

			error(socket, error) {
				console.error(error);
			}
		}
	})
} 

function readResponse(data:Buffer){
	let buff=Buffer.alloc(0);
	buff= Buffer.concat([buff, data]);
	while (true){
   		let msgLen = buff.readInt32BE(0);
		if (buff.length < 4 + msgLen)
        break;
		let rpcmsg=buff.subarray(4,4+msgLen)
		console.log(rpcmsg.toString)
		buff=buff.subarray(4+msgLen)
	}
}

export function SearchPopup({ isOpen, onClose }: { isOpen: boolean; onClose: () => void }) {
	const [query, setQuery] = useState("");

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


