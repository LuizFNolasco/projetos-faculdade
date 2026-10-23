"use client";

import { useEffect, useState } from "react";

import { FocoLayout } from "@/components/layout/foco-nav";
import { Trash2 } from "lucide-react";
import {
  calcularTempos,
  formatDuration,
  formatHour,
} from "@/lib/sessao/calculos";
import type { SessaoEstudo } from "@/interfaces/Sessao";
import Link from "next/link";

function LogsContent() {
  const [sessoes, setSessoes] = useState<SessaoEstudo[]>([]);
  const [carregando, setCarregando] = useState(true);
  const [isLogado, setIsLogado] = useState(false);

  useEffect(() => {
    async function load() {
      const meRes = await fetch("/api/auth/me");
      const me = await meRes.json();

      if (me.usuario) {
        setIsLogado(true);
        const res = await fetch("/api/sessoes");
        setSessoes(await res.json());
      } else {
        setIsLogado(false);
      }
      setCarregando(false);
    }
    load();
  }, []);

  async function handleDelete(id: string) {
    if (!confirm("Tem certeza que deseja excluir esta sessão?")) return;

    try {
      const res = await fetch(`/api/sessoes/${id}`, {
        method: "DELETE",
      });
      if (res.ok) {
        setSessoes((prev) => prev.filter((s) => s.id !== id));
      } else {
        alert("Erro ao excluir sessão.");
      }
    } catch {
      alert("Erro de conexão ao excluir sessão.");
    }
  }

  if (carregando) {
    return (
      <div className="flex flex-col gap-4">
        {[1, 2, 3].map((i) => (
          <div
            key={i}
            className="h-28 animate-pulse rounded-2xl bg-[#112031]"
          />
        ))}
      </div>
    );
  }

  if (!isLogado) {
    return (
      <div className="flex flex-col items-center justify-center rounded-2xl border border-white/5 bg-[#0d1826]/80 p-12 text-center mt-12">
        <p className="mb-6 text-[#8fa8c4]">
          Faça login para salvar o seu histórico de foco.
        </p>
        <Link
          href="/cadastro"
          className="rounded-full bg-[#04D939] px-8 py-3 font-semibold text-[#012340] transition-all hover:brightness-110"
        >
          CRIAR MINHA CONTA
        </Link>
      </div>
    );
  }

  if (sessoes.length === 0) {
    return (
      <div className="flex flex-1 flex-col items-center justify-center rounded-2xl border border-dashed border-white/10 p-12 text-center">
        <p className="text-[#8fa8c4]">Nenhuma sessão registrada ainda.</p>
        <p className="mt-2 text-sm text-[#8fa8c4]">
          Inicie um timer em Foco para ver seu histórico aqui.
        </p>
      </div>
    );
  }

  return (
    <div className="flex max-h-[420px] flex-1 flex-col gap-4 overflow-y-auto pr-1">
      {sessoes.map((sessao) => {
        const config = sessao.configuracao;
        const { ativo, descanso } = calcularTempos(sessao, config);
        const dataRef = new Date(sessao.inicio);

        return (
          <article
            key={sessao.id}
            className="group relative grid gap-6 rounded-2xl border border-white/5 bg-[#0d1826]/80 p-5 md:grid-cols-[120px_1fr_100px] md:items-center hover:bg-[#112031] transition-colors"
          >
            <button
              onClick={() => handleDelete(sessao.id)}
              className="absolute right-4 top-4 hidden size-8 items-center justify-center rounded-full bg-red-500/10 text-red-400 opacity-0 transition-all hover:bg-red-500 hover:text-white group-hover:flex group-hover:opacity-100"
              title="Excluir log"
            >
              <Trash2 size={16} />
            </button>
            <div className="flex flex-col items-center gap-2 text-center">
              <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-[#8fa8c4]">
                Ciclos concluídos
              </p>
              <div className="flex size-20 items-center justify-center rounded-full border-2 border-[#04D939] text-3xl font-bold text-white">
                {sessao.ciclosCompletos}
              </div>
            </div>

            <div className="grid gap-4 sm:grid-cols-2">
              <div className="space-y-3">
                <div>
                  <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-[#8fa8c4]">
                    Começou:
                  </p>
                  <p className="text-sm font-semibold text-white">
                    {formatHour(sessao.inicio)}
                  </p>
                </div>
                <div>
                  <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-[#8fa8c4]">
                    Configuração:
                  </p>
                  <p className="text-sm font-semibold text-white">
                    {config?.nome ?? "—"}
                  </p>
                </div>
              </div>

              <div className="space-y-3">
                <div>
                  <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-[#8fa8c4]">
                    Tempo ativo:
                  </p>
                  <p className="text-sm font-semibold text-white">
                    {formatDuration(ativo)}
                  </p>
                </div>
                <div>
                  <p className="text-[11px] font-semibold uppercase tracking-[0.18em] text-[#8fa8c4]">
                    Tempo descanso:
                  </p>
                  <p className="text-sm font-semibold text-white">
                    {formatDuration(descanso)}
                  </p>
                </div>
              </div>
            </div>

            <div className="flex items-center justify-center md:justify-end">
              <div className="text-center md:text-right">
                <p className="text-4xl font-bold leading-none text-white">
                  {dataRef.toLocaleDateString("pt-BR", { day: "2-digit" })}
                </p>
                <p className="mt-1 text-xs font-semibold tracking-[0.2em] text-[#8fa8c4]">
                  {dataRef
                    .toLocaleDateString("pt-BR", { month: "long" })
                    .toUpperCase()}
                </p>
              </div>
            </div>
          </article>
        );
      })}
    </div>
  );
}

export default function LogsPage() {
  return (
    <FocoLayout activeTab="logs">
      <LogsContent />
    </FocoLayout>
  );
}
