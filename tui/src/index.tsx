import { createCliRenderer } from "@opentui/core";
import { createRoot } from "@opentui/react";
import { App } from "./App";
import { startReader } from "./client/client";

// Ensure startReader is imported to initialize connection
startReader().catch(console.error);

const renderer = await createCliRenderer();
createRoot(renderer).render(<App />);
