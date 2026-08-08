import { Song } from "./song.ts";

export function parseToSong(
  setSong: React.Dispatch<React.SetStateAction<Song>>,
  messageBuffer: Buffer,
) {
  try {
    const data = JSON.parse(messageBuffer.toString());
    if (data.songdata) {
//		setSong(data.songdata)
    setSong((prevSong) => ({
      ...prevSong,
      title: data.songdata.title || prevSong.title,
      artist: data.songdata.artist || prevSong.artist,
      duration: data.songdata.duration || prevSong.duration,
      isLiked: data.songdata.isliked !== undefined ? data.songdata.isliked : prevSong.isLiked,
	  isPlayed:true,
    }));

      //console.log("mekada undefined",data.songdata);
    }else if (data.eventdata){
	// if event is end-file it means song is over, if its property change and the .name is time-pos is means its the time stamp and you should take the .eventdata.data and change the time in the playbar and also the progress bar.
		//console.log("event",data.eventdata.event);
		//console.log("dataname",data.eventdata.name);
		// the is the actual time
		//console.log("datadata",data.eventdata.data);
		
		if (data.eventdata.event === "property-change" && data.eventdata.name === "time-pos") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: data.eventdata.data !== undefined ? data.eventdata.data : prevSong.timestamp
			}));
		} else if (data.eventdata.event === "end-file") {
			setSong((prevSong) => ({
				...prevSong,
				timestamp: 0,
				isPlayed: false
			}));
		}
	}else if (data.replydata){
		if (data.replydata.request_id=="2"){
		setSong((prevSong) => ({
      ...prevSong,
      duration: data.replydata.data|| prevSong.duration,
    }));
		console.log("reply",data.replydata.data);
		}

	}else{
		console.log("no song");
	}
  } catch (e) {
    console.error("Failed to parse JSON-RPC response in parseToSong", e);
  }
}




