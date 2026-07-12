import { useState } from "react";

export function SearchPopup({ isOpen, onClose }: { isOpen: boolean; onClose: () => void }) {
  const [query, setQuery] = useState("");

  if (!isOpen) return null;

  return (
    <box
      position="absolute"
      width="100%"
      height="100%"
      justifyContent="center"
      alignItems="center"
    >
      <box
        width={50}
        height={5}
        backgroundColor="#282828"
        borderStyle="rounded"
        flexDirection="column"
        paddingX={1}
      >
        <text fg="#1DB954"><b>Search</b></text>
        <input
          placeholder="Type here..."
          onInput={setQuery}
          onSubmit={onClose}
          focused={true}
        />
      </box>
    </box>
  );
}
