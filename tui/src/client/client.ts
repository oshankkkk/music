import { createRequest } from "./request";
import type { Song } from "./types";
import type { Socket } from "bun";

let tuiSocket: Socket | null = null;

type RPCCallback = (data: any) => void;
const rpcListeners: Record<string, RPCCallback[]> = {};

export function addRpcListener(type: string, callback: RPCCallback) {
    if (!rpcListeners[type]) rpcListeners[type] = [];
    rpcListeners[type].push(callback);
    return () => {
        if (rpcListeners[type]) {
            rpcListeners[type] = rpcListeners[type].filter(cb => cb !== callback);
        }
    };
}

export function emitRpcResponse(type: string, data: any) {
    if (rpcListeners[type]) {
        rpcListeners[type].forEach(cb => cb(data));
    }
}

const connection:Promise<void> = new Promise((resolve,reject)=>{

	console.log("sokcing -->")
	let buff = Buffer.alloc(0);
	
	Bun.connect({
		unix: "../build/us.socket",
		socket: {
			open(socket) {
				console.log("sokcing")
				tuiSocket = socket;
				resolve();
				console.log("sokcing -->",tuiSocket)
			},
			data(socket, data) {
				buff = Buffer.concat([buff, data]);
				while (true) {
					if (buff.length < 4) break;
					let msgLen = buff.readUInt32LE(0);
					if (buff.length < 4 + msgLen) break;
					
					let rpcmsg = buff.subarray(4, 4 + msgLen);
					try {
                        const msg = JSON.parse(rpcmsg.toString());
                        emitRpcResponse(msg.type, msg);
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
	}).catch(reject);
});

export function startReader(): Promise<void>{
	return connection;
}
export async function rpcCall(method: string,type:string, params: any = {}) {
	await connection;
		let request = createRequest(method, params,type);
		if (!tuiSocket) {
			throw new Error ("Socket not connected");
		} else {
			tuiSocket.write(request);
		}
};
