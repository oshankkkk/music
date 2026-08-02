export async function rpcCall(method: string, params: any = {}): Promise<any> {
	return new Promise((resolve, reject) => {
		let message = {
			jsonrpc: "2.0",
			method,
			params,
			id: 1 // We can just use a static ID for one-off calls
		};

		let request = JSON.stringify(message);
		let buff = Buffer.alloc(0);

		Bun.connect({
			unix: "../build/us.socket",
			socket: {
				open(socket) {
					socket.write(request);
				},
				data(socket, data) {
					buff = Buffer.concat([buff, data]);
					while (true) {
						if (buff.length < 4) break;
						let msgLen = buff.readUInt32LE(0);
						if (buff.length < 4 + msgLen) break;
						let rpcmsg = buff.subarray(4, 4 + msgLen);
						try {
							const response = JSON.parse(rpcmsg.toString());
							resolve(response);
						} catch (e) {
							reject(e);
						}
						// Close after first response since it's a one-off request
						socket.end();
						break; 
					}
				},
				close(socket) {
					// Connection closed
				},
				error(socket, error) {
					reject(error);
				}
			}
		}).catch(reject);
	});
}
