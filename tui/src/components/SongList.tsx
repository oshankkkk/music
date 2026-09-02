import { useRef, useEffect } from "react";
import type { BoxOptions } from "@opentui/react"; // Not strictly typed in the project, we can just use `any` or expand our interface. Wait, let's just use React.ComponentProps<"scrollbox"> if it's available, or just spread it.

interface SongListProps<T> {
  items: T[];
  selectedIndex: number;
  isFocused: boolean;
  renderItem: (item: T, index: number, isSelected: boolean) => React.ReactNode;
  gap?: number;
  marginTop?: number;
}

export function SongList<T>({ items, selectedIndex, isFocused, renderItem, gap = 0, marginTop = 0 }: SongListProps<T>) {
  const scrollboxRef = useRef<any>(null);

  useEffect(() => {
    if (scrollboxRef.current && typeof scrollboxRef.current.scrollChildIntoView === 'function') {
      scrollboxRef.current.scrollChildIntoView(`list-item-${selectedIndex}`);
    }
  }, [selectedIndex, items.length]);

  return (
    <scrollbox
      ref={scrollboxRef}
      flexGrow={1}
      flexDirection="column"
      gap={gap}
      marginTop={marginTop}
      focused={isFocused}
      style={{
        scrollbarOptions: {
          showArrows: false,
          trackOptions: {
            foregroundColor: "#1DB954",
            backgroundColor: "#282828",
          },
        },
      }}
    >
      {items.map((item, i) => (
        <box key={i} id={`list-item-${i}`} width="100%">
          {renderItem(item, i, selectedIndex === i)}
        </box>
      ))}
    </scrollbox>
  );
}
