import { useState } from "react";

export function SearchPopup({ isOpen, onClose }: { isOpen: boolean; onClose: () => void }) {
	const [query, setQuery] = useState("");

	if (!isOpen) return null;

	async function handleSubmit(){
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
					console.log(data.toString);
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
          onSubmit={handleSubmit}
          focused={true}
        />
      </box>
    </box>
  );
}


