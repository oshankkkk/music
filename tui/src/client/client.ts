export function startReader() {
	let buff = Buffer.alloc(0);

	Bun.connect({
		unix: "../build/us.socket",
		socket: {
			open(socket) {},
			data(socket, data) {
				buff = Buffer.concat([buff, data]);
				while (true) {
					if (buff.length < 4) break;
					let msgLen = buff.readUInt32LE(0);
					if (buff.length < 4 + msgLen) break;
					
					let rpcmsg = buff.subarray(4, 4 + msgLen);
					try {
						const response = JSON.parse(rpcmsg.toString());
						console.log(response);
					} catch (e) {
						console.error("Failed to parse message", e);
					}
					buff = buff.subarray(4 + msgLen);
				}
			},
			close(socket) {},
			error(socket, error) {
				console.error("Socket error in reader:", error);
			}
		}
	}).catch(console.error);
}

export async function rpcCall(method: string, params: any = {}): Promise<void> {
	return new Promise((resolve, reject) => {
		let message = {
			jsonrpc: "2.0",
			method,
			params,
			id: 1
		};

		let request = JSON.stringify(message);

		Bun.connect({
			unix: "../build/us.socket",
			socket: {
				open(socket) {
					socket.write(request);
				},
				data(socket, data) {},
				close(socket) {},
				error(socket, error) {
					reject(error);
				}
			}
		}).catch(reject);
	});
}
