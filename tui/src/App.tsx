import { Sidebar } from "./components/Sidebar";
import { MainContent } from "./components/MainContent";
import { PlaylistDetail } from "./components/PlaylistDetail";
import { ContextPanel } from "./components/ContextPanel";
import { Playbar } from "./components/Playbar";
import { SongPlaySearch } from "./components/SongPlaySearch";
import { CreatePlaylistPopup } from "./components/CreatePlaylistPopup";
import { RenamePlaylistPopup } from "./components/RenamePlaylistPopup";
import { QueuePopup } from "./components/QueuePopup";
import { SelectPlaylistPopup } from "./components/SelectPlaylistPopup";
import { rpcCall } from "./client/client";
import { useAppLogic } from "./hooks/useAppLogic";

export function App() {
  const {
    song, setSong,
    playlists, setPlaylists,
    queue, setQueue,
    recentlyPlayed,
    selectedPlaylist, setSelectedPlaylist,
    focusArea, setFocusArea,
    isSearchOpen, setIsSearchOpen,
    isPlaylistSearchOpen, setIsPlaylistSearchOpen,
    isCreatePlaylistOpen, setIsCreatePlaylistOpen,
    playlistToRename, setPlaylistToRename,
    isQueueOpen, setIsQueueOpen,
    isAddSongOpen, setIsAddSongOpen,
    isPlaying, togglePlay
  } = useAppLogic();

  return (
    <box flexDirection="column" width="100%" height="100%" backgroundColor="#000000">
      <box width="100%" height={1} justifyContent="center" alignItems="center" backgroundColor="#181818">
        <text fg="#b3b3b3">Search: space+s | Add to Playlist: space+a | Exit: ctrl+c</text>
      </box>
      <box flexDirection="row" width="100%" flexGrow={1}>
        <Sidebar 
          isFocused={focusArea === "sidebar" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen && !isPlaylistSearchOpen && !playlistToRename} 
          playlists={playlists} 
          setPlaylists={setPlaylists}
          onSelectPlaylist={(p) => { setSelectedPlaylist(p); setFocusArea("playlist"); }}
          onDeletePlaylist={(id) => {
            rpcCall("lib-removeplaylist", "lib", { playlistid: parseInt(id) });
          }}
          onRenamePlaylist={(p) => {
            setPlaylistToRename(p);
          }}
        />
        {selectedPlaylist ? (
          <PlaylistDetail 
            playlist={selectedPlaylist} 
            isFocused={focusArea === "playlist" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen && !isPlaylistSearchOpen && !playlistToRename} 
            isPlaylistSearchOpen={isPlaylistSearchOpen}
            onBack={() => { setSelectedPlaylist(null); setFocusArea("sidebar"); }} 
          />
        ) : (
          <MainContent focusArea={isSearchOpen || isCreatePlaylistOpen || isQueueOpen || isAddSongOpen || isPlaylistSearchOpen || playlistToRename ? "none" : focusArea} recentlyPlayed={recentlyPlayed} />
        )}
        <ContextPanel />
      </box>
      <Playbar isFocused={focusArea === "none" && !isSearchOpen && !isCreatePlaylistOpen && !isQueueOpen && !isAddSongOpen && !isPlaylistSearchOpen && !playlistToRename} isPlaying={isPlaying} onTogglePlay={togglePlay} song={song} setSong={setSong} />
      <SongPlaySearch isOpen={isSearchOpen}/>
      <CreatePlaylistPopup 
        isOpen={isCreatePlaylistOpen} 
        onClose={() => setIsCreatePlaylistOpen(false)} 
        onSubmit={(name) => {
          rpcCall("lib-createplaylist", "lib", { title: name });
        }} 
      />
      <RenamePlaylistPopup
        playlist={playlistToRename}
        onClose={() => setPlaylistToRename(null)}
        onSubmit={(name) => {
          rpcCall("lib-renameplaylist", "lib", { playlistid: parseInt(playlistToRename!.id), newname: name });
        }}
      />
      <QueuePopup isOpen={isQueueOpen} onClose={() => setIsQueueOpen(false)} queue={queue} setQueue={setQueue} />
      <SelectPlaylistPopup
        isOpen={isAddSongOpen}
        playlists={playlists}
        onClose={() => setIsAddSongOpen(false)}
        onSelect={(playlistId) => {
          rpcCall("lib-addsongtoplaylist", "lib", { playlistid: parseInt(playlistId), songid: song.id });
          if (selectedPlaylist && selectedPlaylist.id === playlistId) {
            rpcCall("lib-getplaylistsongs", "lib", { playlistid: parseInt(playlistId) });
          }
          setIsAddSongOpen(false);
        }}
      />
    </box>
  );
}
