"use client";

import { Pause, Play, RotateCcw, StepForward } from "lucide-react";

type TimerControlsProps = {
  pausado: boolean;
  confirmacaoPausaAberta: boolean;
  isInitialState?: boolean;
  onReset: () => void;
  onRequestPause: () => void;
  onResume: () => void;
  onSkip: () => void;
};

export function TimerControls({
  pausado,
  confirmacaoPausaAberta,
  isInitialState = false,
  onReset,
  onRequestPause,
  onResume,
  onSkip,
}: TimerControlsProps) {
  const pauseAtivo = confirmacaoPausaAberta || (pausado && !isInitialState);

  return (
    <div className="mb-4 flex w-full items-center justify-center gap-6">
      <button
        type="button"
        onClick={onReset}
        className="flex w-32 cursor-pointer items-center justify-center rounded-full border border-[#04D939] bg-[#04D939]/10 py-2.5 font-medium text-[#04D939] transition-all hover:bg-[#04D939]/25 active:bg-[#04D939]/40"
        aria-label="Reiniciar"
      >
        <RotateCcw size={20} strokeWidth={2} />
      </button>

      <button
        type="button"
        onClick={pausado ? onResume : onRequestPause}
        className={
          pauseAtivo
            ? "flex w-40 cursor-pointer items-center justify-center gap-2 rounded-full border border-[#BC2F32] bg-[#BC2F32]/15 py-2.5 text-sm font-medium tracking-wide text-[#ff8a8a] transition-all hover:border-[#04D939] hover:bg-[#04D939]/15 hover:text-[#04D939] active:bg-[#04D939]/30"
            : "flex w-40 cursor-pointer items-center justify-center gap-2 rounded-full border border-[#04D939] bg-[#04D939]/10 py-2.5 text-sm font-medium tracking-wide text-[#04D939] transition-all hover:border-[#BC2F32] hover:bg-[#BC2F32]/15 hover:text-[#BC2F32] active:bg-[#BC2F32]/30"
        }
      >
        {isInitialState && pausado ? (
          <>
            <Play size={18} fill="currentColor" strokeWidth={0} />
            INICIAR
          </>
        ) : pausado ? (
          <>
            <Play size={18} fill="currentColor" strokeWidth={0} />
            RETOMAR
          </>
        ) : (
          <>
            <Pause size={18} fill="currentColor" strokeWidth={0} />
            PAUSAR
          </>
        )}
      </button>

      <button
        type="button"
        onClick={onSkip}
        className="flex w-32 cursor-pointer items-center justify-center rounded-full border border-[#04D939] bg-[#04D939]/10 py-2.5 font-medium text-[#04D939] transition-all hover:bg-[#04D939]/25 active:bg-[#04D939]/40"
        aria-label="Pular fase"
      >
        <StepForward size={20} strokeWidth={2} />
      </button>
    </div>
  );
}
