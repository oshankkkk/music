import { createRequest } from "./request";
import { Song} from "./song.ts";
import { parseToSong} from "./response";
import type { Socket } from "bun";

let tuiSocket: Socket | null = null;

export function startReader(
song: Song,
setSong: React.Dispatch<React.SetStateAction<Song>>,
) {
	let buff = Buffer.alloc(0);
	
	Bun.connect({
		unix: "../build/us.socket",
		socket: {
			open(socket) {
				tuiSocket = socket;
			},
			data(socket, data) {
				buff = Buffer.concat([buff, data]);
				while (true) {
					if (buff.length < 4) break;
					let msgLen = buff.readUInt32LE(0);
					if (buff.length < 4 + msgLen) break;
					
					let rpcmsg = buff.subarray(4, 4 + msgLen);
					try {
						parseToSong(setSong,rpcmsg);
						//console.log("meh song obj",song);
						//console.log("here comes the response");
					} catch (e) {
						console.error("Failed to parse message", e);
					}
					buff = buff.subarray(4 + msgLen);
				}
			},
			close(socket) {
				tuiSocket = null;
			},
			error(socket, error) {
				console.error("Socket error in reader:", error);
				tuiSocket = null;
			}
		}
	}).catch(console.error);
}

export function rpcCall(method: string, params: any = {}) {
		let request = createRequest(method, params);
		if (!tuiSocket) {
			console.log("yoyo")
			throw new Error ("Socket not connected");
		} else {
			tuiSocket.write(request);
		}
	};




