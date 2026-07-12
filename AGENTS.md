# Spotify Terminal Clone - Implementation Plan

This document outlines the step-by-step plan to build the Spotify Terminal UI using OpenTUI (`@opentui/react` and `@opentui/core`). The approach is iterative, starting with the foundational structural layout before moving on to individual UI components and styling.

## Tech Stack & Core Concepts
- **Framework**: OpenTUI (React bindings)
- **Layout Engine**: Yoga (Flexbox primitives like `flexDirection`, `flexGrow`, `justifyContent`)
- **Theme**: Spotify Dark (Black backgrounds, white/gray text, `#1DB954` green accent)
- **Images**: Rendered as solid colored blocks using Unicode block characters (e.g., `█`, `▓`)

---

## Phase 1: Foundational Layout (The Shell)
**Goal:** Establish the macro-structure of the application.
- Replace the default `src/index.tsx` content with the main layout shell.
- The root container will use `<box flexDirection="column" width="100%" height="100%">` (or rely on full terminal dimensions).
- **Top Section (Main Area):**
  - `<box flexDirection="row" width="100%" flexGrow={1}>`
  - **Left Column (Sidebar):** `<box width={30}>` (fixed width, maybe subtle right border)
  - **Center Column (Main Content):** `<scrollbox flexGrow={1}>` (takes up remaining space, scrollable)
  - **Right Column (Context Panel):** `<box width={35}>` (fixed width, left border)
- **Bottom Section (Playbar):**
  - `<box flexDirection="row" width="100%" height={3}>` (fixed height)
- *Testing:* Temporarily add different background colors to these 4 zones to ensure the flexbox layout responds correctly to the terminal size.

## Phase 2: Bottom Playbar
**Goal:** Implement the static playback controls area.
- Add three sections within the playbar's row layout:
  - **Left (Now Playing):** `flexDirection="row"`, fixed width. Unicode cover art block + Track Title + Artist Name + Like text-icon.
  - **Center (Controls & Progress):** `flexDirection="column"`, `flexGrow={1}`, `alignItems="center"`.
    - Top row: `[<] [Play] [>]` (text-based controls).
    - Bottom row: Progress bar string e.g., `1:37 ───●───── 3:55`.
  - **Right (Extra Controls):** `flexDirection="row"`, fixed width, `justifyContent="flex-end"`. Queue + Volume string (`Vol ──●─`).

## Phase 3: Left Sidebar (Your Library)
**Goal:** Build the scrollable list of playlists/albums.
- **Header:** "Your Library" (plain text, bolded/highlighted).
- **List Container:** `<scrollbox>` (or just a vertical `<box>`).
- **List Items (Hardcoded):** Create a reusable `LibraryItem` component.
  - `<box flexDirection="row" gap={1}>`
  - Colored block placeholder.
  - `<box flexDirection="column">` containing Title and Subtitle ("Playlist · 234 songs").

## Phase 4: Right Context Panel
**Goal:** Display context-aware information for the currently active item.
- **Cover Art:** A large square block composed of Unicode characters at the top.
- **Info Card:**
  - Name (large text/bold).
  - "Verified Artist" badge (if applicable).
  - Monthly listeners (gray text).
- **Actions:** Text-based "Follow" button.
- **Bio/About:** A small snippet of text.

## Phase 5: Main Content Area
**Goal:** Build the central scrollable grids and sections.
- **Greeting:** "Good evening" (Large header).
- **Quick-Access Grid:**
  - A grid of 6-8 items. Use flexbox with wrapping if supported, or hardcode rows of items.
  - Card style: horizontal layout, colored block left, text right.
- **Standard Sections ("Made for you", "Your top mixes"):**
  - Section Header + "Show all" on the right.
  - Horizontal list of cards. Card style: vertical layout, square colored block top, 2 lines of text below.
- *Note:* Exclude the top navigation bar entirely per the specification.

## Phase 6: Polish and Theming
**Goal:** Ensure ~90% visual fidelity with the static constraints.
- Apply exact Spotify colors (backgrounds, `#1DB954` green for active elements, `#b3b3b3` for secondary text).
- Tweak paddings, margins, and flex gaps.
- Remove any visual debugging borders/backgrounds used in Phase 1.
- Ensure terminal resize events are handled gracefully (using OpenTUI's built-in flexbox logic).

---

## Implementation Log (Session Summary)

**Status:** All 6 Phases have been successfully completed, with added interactive state.

1. **Plan Creation:** Authored this iterative `frontendPlan.md` based on `AGENTS.md` and the OpenTUI docs.
2. **Phase 1 Implementation:** Updated `src/index.tsx` with the foundational Yoga flexbox layout (3 columns on top, 1 fixed playbar on bottom). Added a `useKeyboard` exit hook.
3. **Phases 2-5 Implementation:** Built the hardcoded static UI components directly into the monolithic `index.tsx` shell, including:
   - Bottom playbar with time progress (`──────●─────`).
   - Left sidebar with library item rows.
   - Right context panel with a block-character grid for cover art and a verified artist badge.
   - Main content area with a Quick Access grid and "Made for you" cards.
4. **First Commit:** Created a Git commit `feat: add initial UI layout shell`.
5. **Phase 6 Component Refactor:** Extracted the inline elements from `src/index.tsx` into modular React components stored in `src/components/` to improve long-term maintainability:
   - `Sidebar.tsx`
   - `MainContent.tsx`
   - `ContextPanel.tsx`
   - `Playbar.tsx`
6. **Second Commit:** Created a Git commit `feat: extract layout sections into modular components`.
7. **Playbar Interactivity:** Refactored `Playbar.tsx` to use component state instead of hardcoded variables. Added `useKeyboard` to parse `space` (play/pause), `j/k` (volume down/up), and `h/l` (prev/next). Added highlight animations mapping numeric `useTimeline` values to hex strings for instant feedback flashes.
8. **Third Commit:** Created a Git commit `feat: add stateful playbar controls and keybindings`.
9. **Numeric Volume Bar:** Replaced the segmented volume bar with a fast-rendering numeric percentage string (e.g. `50%`) to improve the feedback loop when pressing `j/k`.
10. **Fourth Commit:** Created a Git commit `feat: replace volume bar with numeric percentage`.
11. **Sidebar Focus & Navigation:** Implemented sequence detection in `src/index.tsx` to detect `space p`, which sets `isSidebarFocused` to `true`. Updated `Sidebar.tsx` to conditionally capture `j/k` and navigate up/down the playlists with visual highlights when focused. `Playbar.tsx` gracefully yields volume controls when the sidebar is focused.
12. **Fifth Commit:** Created a Git commit `feat: add sidebar focus and playlist navigation`.
13. **Multi-Area Focus System:** Replaced the boolean `isSidebarFocused` in `src/index.tsx` with a `focusArea` enum (`"none" | "sidebar" | "quick-access" | "mixes"`). Added sequence detection for `space n` (quick-access grid) and `space m` (mixes section). Pressing `escape` resets focus to `"none"`.
14. **Navigable Main Content:** Rewrote `MainContent.tsx` with data-driven arrays and local navigation state:
    - **Quick Access grid** (`space n`): Navigate with `j/k` (rows) and `h/l` (columns). Selected card gets brighter background (`#3e3e3e`) and green title (`#1DB954`).
    - **Mixes section** (`space m`): Navigate horizontally with `h/l`. Selected card gets lighter background (`#282828`) and green title.
15. **Playbar Focus Cleanup:** Simplified `Playbar.tsx` prop from `isSidebarFocused` to `isFocused` (only `true` when `focusArea === "none"`). All playbar keybindings are now gated behind a single early-return check.
16. **Sixth Commit:** Created a Git commit `feat: add navigable quick-access and mixes sections`.

17. **Global Play/Pause & Leader Key:** Replaced the string-buffer sequence detector in `src/index.tsx` with a timer-based leader key pattern using `useRef`. Pressing `space` alone (after 300ms timeout with no follow-up) toggles play/pause globally regardless of focus area. If `p/n/m/b` arrives within the 300ms window, it's treated as a focus sequence instead.
18. **`space b` Playbar Focus:** Added `space b` as a keybinding to return focus to the bottom playbar (`focusArea = "none"`).
19. **Lifted `isPlaying` State:** Moved `isPlaying` from `Playbar.tsx` local state to `App` in `index.tsx`. Playbar now receives it as a prop. Removed `space` from Playbar's local `useKeyboard` handler. Added a `useEffect` that watches `isPlaying` changes and triggers the play icon flash animation globally (skipping first render).
20. **Seventh Commit:** Created a Git commit `feat: add global play/pause and space+b playbar focus`.

### Keybinding Reference

| Sequence | Action | Scope |
|---|---|---|
| `space` (alone) | Toggle play/pause | Global |
| `space p` | Focus sidebar | Global |
| `space n` | Focus quick-access grid | Global |
| `space m` | Focus mixes section | Global |
| `space b` | Focus bottom playbar | Global |
| `escape` | Unfocus / exit app | Global |
| `Ctrl+C` | Exit app | Global |
| `j/k` | Navigate down/up | Sidebar, Quick-access |
| `h/l` | Navigate left/right | Quick-access, Mixes |
| `j/k` | Volume down/up | Playbar focused |
| `h/l` | Prev/next track | Playbar focused |

The application completely mimics the ~90% visual static layout of the Spotify desktop client in a terminal environment while injecting robust local keyboard interactivity!

21. **Space Key Crash Fix:** The app crashed when pressing `space` alone because `togglePlay()` was called from a raw `setTimeout` callback, triggering React state updates and OpenTUI `useTimeline` animations outside the renderer's managed event processing cycle. Replaced the `setTimeout` approach with a timestamp-based ref (`spaceTimestampRef`) and a `setInterval` poller that safely defers the play toggle through a React `useEffect` (via a `playToggleTick` state counter). Also fixed a secondary bug where `escape` was grouped with `ctrl+c` in the same exit handler, making the escape-unfocus code dead — `escape` now correctly resets focus without killing the app.

## Phase 7: Top Bar & Search Popup
**Goal:** Add a global top bar above the 3-column layout indicating the search shortcut, and implement a toggleable search input popup.
- **Top Bar:** Insert a full-width `<box>` at the top of `App` component with height 1, centered text: "Search press space+s".
- **State:** Add `isSearchOpen` state to track if the search popup is visible.
- **Keybindings:** Update `useKeyboard` in `App`:
  - `space s` toggles `isSearchOpen` to `true`.
  - `escape` toggles `isSearchOpen` to `false` and clears search input (or hides it).
  - Main app keyboard events are early-returned when `isSearchOpen` is true to allow the popup input to capture typing smoothly.
- **Search Popup:**
  - Created `SearchPopup.tsx` positioned absolutely with a centered box and `@opentui/core`'s `<input>` component handling `onInput` and `onSubmit`.

## Phase 8: Playbar Complex State & Interactive Mouse Events
**Goal:** Enhance the Playbar with comprehensive track state and interactive mouse controls.
- **Complex State:** Added a structured `song` state object to `Playbar.tsx` containing `title`, `description`, `artist`, `timestamp`, `duration`, `albumArtUrl`, `isLiked`, `playlists`, and `isPlayed`. Added an `isMicActive` state and maintained the `volume` state.
- **Mouse Interactivity (`onMouseDown`):**
  - **Play/Pause:** Clicking the play/pause icon now toggles the `song.isPlayed` local state and propagates to the global `isPlaying` state via a newly passed `onTogglePlay` prop (calling `togglePlay` from `App` in `index.tsx`).
  - **Next/Prev Track:** Clicking `|<` or `>|` modifies the dummy `song` object (e.g., resets timestamp and changes title).
  - **Mic/Like/Volume:** Toggles `isMicActive` (turning it green) and `song.isLiked` (♥/♡). Clicking the `Vol` text label toggles volume between 0 and 50 instantly.
- **Eighth Commit:** Added comprehensive React state and mouse handlers to the bottom playbar component.
