import { Song } from "./song.ts";

export function parseToSong(
  setSong: React.Dispatch<React.SetStateAction<Song>>,
  messageBuffer: Buffer,
) {
  try {
    const data = JSON.parse(messageBuffer.toString());
    if (data.songdata) {
		setSong(data.songdata)
   //   setSong((prevSong) => ({
   //     ...prevSong,
   //     title: data.result.title || prevSong.title,
   //     artist: data.result.artist || prevSong.artist,
   //     duration: data.result.duration || prevSong.duration,
   //     isLiked: data.result.isliked !== undefined ? data.result.isliked : prevSong.isLiked,
   //   }));

      console.log("set song wuna");
      console.log("mekada undefined",data.songdata);
    }else if (data.eventdata){
// if event is end-file it means song is over, if its property change and the .name is time-pos is means its the time stamp and you should take the .eventdata.data and change the time in the playbar and also the progress bar.
		console.log("event",data.eventdata.event);
		console.log("dataname",data.eventdata.name);
		// the is the actual time
		console.log("datadata",data.eventdata.data);


	}else if (data.replydata){
		console.log("reply");
	}else{
		console.log("no song");
	}
  } catch (e) {
    console.error("Failed to parse JSON-RPC response in parseToSong", e);
  }
}




