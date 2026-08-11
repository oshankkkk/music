import { QuickAccessGrid } from "./QuickAccessGrid";
import { MadeForYou } from "./MadeForYou";
import { RecentlyPlayed } from "./RecentlyPlayed";

export function MainContent({ focusArea }: { focusArea: string }) {
  return (
    <box flexGrow={1} backgroundColor="#000000" flexDirection="column" paddingX={3} paddingTop={1}>
      <RecentlyPlayed isFocused={focusArea === "recently-played"} />
    </box>
  );
}
