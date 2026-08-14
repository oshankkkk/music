import type { Song } from "./types";

export function parseToSong(
  setSong: React.Dispatch<React.SetStateAction<Song>>,
  messageBuffer: Buffer,
) {
  try {
    const data = JSON.parse(messageBuffer.toString());
    if (data.type==="song") {

    setSong((prevSong) => ({
      ...prevSong,
      title: data.result.title || prevSong.title,
      artist: data.result.artist || prevSong.artist,
      duration: data.result.duration || prevSong.duration,
      isLiked: data.result.isliked !== undefined ? data.result.isliked : prevSong.isLiked,
	  isPlayed:true,
    }));

    }else if (data.type ==="mpv-event"){
		if (data.result.event === "property-change" && data.result.name === "time-pos") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: data.result.data !== undefined ? data.result.data : prevSong.timestamp
			}));
		} else if (data.result.event === "end-file") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: 0,
				isPlayed: false
			}));
		}
    }else if (data.type ==="playlist"){
		console.log(data.result.id)	
		console.log(data.result.name)	
		console.log(data.result.songcount)	
		console.log(data.result.songtime)	
		console.log(data.result.songs)
    }else if (data.type ==="queue"){
		if (data.result.songs){
	// this is a list of all the songs in a queue
		}
	}else if (data.type==="mpv-reply"){
		if (data.result.request_id=="2"){
		setSong((prevSong) => ({
      ...prevSong,
      duration: data.result.data|| prevSong.duration,
    }));
		console.log("reply",data.result.data);
		}

	}else{
		console.log("this is prolly unknown type",data.type);
	}
  } catch (e) {
    console.error("Failed to parse JSON-RPC result in parseToSong", e);
  }
}




