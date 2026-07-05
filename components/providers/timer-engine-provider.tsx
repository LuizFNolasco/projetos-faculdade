"use client";

import {
  createContext,
  useCallback,
  useContext,
  useEffect,
  useState,
  type ReactNode,
} from "react";

import { useTimerConfig } from "@/components/providers/timer-config-provider";
import {
  avancarFase,
  criarEstadoInicial,
  duracaoFaseEmSegundos,
  type TimerEstado,
} from "@/lib/timer/engine";


type TimerEngineContextValue = {
  estado: TimerEstado | null;
  confirmacaoPausaAberta: boolean;
  handleReset: () => void;
  handleRequestPause: () => void;
  handleConfirmarPausa: () => void;
  handleCancelarPausa: () => void;
  handleResume: () => void;
  handleSkip: () => void;
};

const TimerEngineContext = createContext<TimerEngineContextValue | null>(null);

export function TimerEngineProvider({ children }: { children: ReactNode }) {
  const { configAtiva, usuarioLogado } = useTimerConfig();
  
  const [estado, setEstado] = useState<TimerEstado | null>(null);
  const [sessaoId, setSessaoId] = useState<string | null>(null);
  const [confirmacaoPausaAberta, setConfirmacaoPausaAberta] = useState(false);

  const persistirSessao = useCallback(
    async (novoEstado: TimerEstado) => {
      const payload = {
        ciclosCompletos: novoEstado.ciclosCompletos,
        faseAtual: novoEstado.faseAtual,
        cicloAtual: novoEstado.cicloAtual,
      };

      if (usuarioLogado && sessaoId) {
        await fetch(`/api/sessoes/${sessaoId}`, {
          method: "PATCH",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify(payload),
        });
      }
    },
    [usuarioLogado, sessaoId],
  );

  useEffect(() => {
    if (!configAtiva) return;

    setEstado((prev) => {
      if (prev) return prev;
      return criarEstadoInicial(configAtiva);
    });

    async function iniciarSessao() {
      if (sessaoId) return;

      if (usuarioLogado) {
        const res = await fetch("/api/sessoes", {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({ configuracaoId: configAtiva!.id }),
        });
        if (res.ok) {
          const sessao = (await res.json()) as any;
          setSessaoId(sessao.id);
        }
      }
    }

    iniciarSessao();
  }, [configAtiva, usuarioLogado]);

  useEffect(() => {
    if (!estado || estado.pausado || !configAtiva) return;

    const interval = setInterval(() => {
      setEstado((prev) => {
        if (!prev || prev.pausado) return prev;

        if (prev.tempoRestanteSeg <= 1) {
          const proximo = avancarFase(prev, configAtiva);
          persistirSessao(proximo);
          return proximo;
        }

        return { ...prev, tempoRestanteSeg: prev.tempoRestanteSeg - 1 };
      });
    }, 1000);

    return () => clearInterval(interval);
  }, [estado?.pausado, configAtiva, persistirSessao]);

  function handleReset() {
    if (!configAtiva || !estado) return;
    
    const duracao = duracaoFaseEmSegundos(estado.faseAtual, configAtiva);
    const novo = {
      ...estado,
      tempoRestanteSeg: duracao,
      pausado: true,
    };
    
    setEstado(novo);
    persistirSessao(novo);
  }

  function handleRequestPause() {
    if (estado?.faseAtual === "FOCO") {
      setConfirmacaoPausaAberta(true);
    } else {
      setEstado((prev) => (prev ? { ...prev, pausado: true } : prev));
    }
  }

  function handleConfirmarPausa() {
    setConfirmacaoPausaAberta(false);
    setEstado((prev) => (prev ? { ...prev, pausado: true } : prev));
  }

  function handleCancelarPausa() {
    setConfirmacaoPausaAberta(false);
  }

  function handleResume() {
    setEstado((prev) => (prev ? { ...prev, pausado: false } : prev));
  }

  function handleSkip() {
    if (!configAtiva || !estado) return;
    const proximo = avancarFase(estado, configAtiva);
    setEstado(proximo);
    persistirSessao(proximo);
  }

  return (
    <TimerEngineContext.Provider
      value={{
        estado,
        confirmacaoPausaAberta,
        handleReset,
        handleRequestPause,
        handleConfirmarPausa,
        handleCancelarPausa,
        handleResume,
        handleSkip,
      }}
    >
      {children}
    </TimerEngineContext.Provider>
  );
}

export function useTimerEngine() {
  const ctx = useContext(TimerEngineContext);
  if (!ctx) {
    throw new Error(
      "useTimerEngine deve ser usado dentro de TimerEngineProvider",
    );
  }
  return ctx;
}
