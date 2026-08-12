import { Song } from "./song.ts";

export function parseToSong(
  setSong: React.Dispatch<React.SetStateAction<Song>>,
  messageBuffer: Buffer,
) {
  try {
    const data = JSON.parse(messageBuffer.toString());
    if (data.type==="song") {

    setSong((prevSong) => ({
      ...prevSong,
      title: data.response.title || prevSong.title,
      artist: data.response.artist || prevSong.artist,
      duration: data.response.duration || prevSong.duration,
      isLiked: data.response.isliked !== undefined ? data.response.isliked : prevSong.isLiked,
	  isPlayed:true,
    }));

      //console.log("mekada undefined",data.response);
    }else if (data.type ==="mpv-event"){
    }else if (data.type ==="playlist"){

    }else if (data.type ==="queue"){
		
		if (data.response.event === "property-change" && data.response.name === "time-pos") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: data.response.data !== undefined ? data.response.data : prevSong.timestamp
			}));
		} else if (data.response.event === "end-file") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: 0,
				isPlayed: false
			}));
		}
	}else if (data.type==="mpv-reply"){
		if (data.response.request_id=="2"){
		setSong((prevSong) => ({
      ...prevSong,
      duration: data.response.data|| prevSong.duration,
    }));
		console.log("reply",data.response.data);
		}

	}else{
		console.log("this is prolly unknown type",data.type);
	}
  } catch (e) {
    console.error("Failed to parse JSON-RPC response in parseToSong", e);
  }
}




