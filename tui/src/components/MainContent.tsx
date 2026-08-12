import { QuickAccessGrid } from "./QuickAccessGrid";
import { MadeForYou } from "./MadeForYou";
import { RecentlyPlayed } from "./RecentlyPlayed";
import type { RecentlyPlayedItem } from "../client/types";

export function MainContent({ focusArea, recentlyPlayed }: { focusArea: string, recentlyPlayed: RecentlyPlayedItem[] }) {
  return (
    <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
      <RecentlyPlayed isFocused={focusArea === "recently-played"} recentlyPlayed={recentlyPlayed} />
    </box>
  );
}
