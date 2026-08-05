export function parseResponse(messageBuffer: Buffer): any {
    return JSON.parse(messageBuffer.toString());
}
