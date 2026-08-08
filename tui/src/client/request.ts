let nextRequestId = 3;

export function createRequest(method: string, params: any = {}): string {
    const message = {
        jsonrpc: "2.0",
        method,
        params,
        id: nextRequestId++
    };
    return JSON.stringify(message);
}

