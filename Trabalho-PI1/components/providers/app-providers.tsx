"use client";

import { TimerConfigProvider } from "./timer-config-provider";
import { TimerEngineProvider } from "./timer-engine-provider";

export function AppProviders({ children }: { children: React.ReactNode }) {
  return (
    <TimerConfigProvider>
      <TimerEngineProvider>
        {children}
      </TimerEngineProvider>
    </TimerConfigProvider>
  );
}
