type Song = { title: string; duration: number; artist: string; timestamp: number; isLiked: boolean; isPlayed: boolean; };
const messageBuffer = Buffer.from('{"jsonrpc":"2.0","type":"2.0","replydata":{"data":372.715083,"request_id":"2","error":"success"}}');
const data = JSON.parse(messageBuffer.toString());
let song: Song = { title: "Test", duration: 212, artist: "A", timestamp: 0, isLiked: false, isPlayed: false };

if (data.replydata) {
    if (data.replydata.request_id == "2") {
        song = {
            ...song,
            duration: data.replydata.data || song.duration,
        };
    }
}
console.log(song);
