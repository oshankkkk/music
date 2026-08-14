let nextRequestId = 3;

export function createRequest(method: string, params: any = {},type:string): string {
    const message = {
        jsonrpc: "2.0",
		type,
        method,
        params,
        id: nextRequestId++
    };
    return JSON.stringify(message);
}

