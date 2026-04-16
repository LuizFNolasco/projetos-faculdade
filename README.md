# Trabalho de SO

## Implementação de Escalonadores de Processos

### Visão Geral

Este projeto implementa diferentes escalonadores de processos (schedulers) em um mini-kernel, incluindo:

- **First Come First Served (FCFS)**
- **Round Robin (RR)**
- **Priority**
- **Completely Fair Scheduler (CFS)**

O foco principal deste README é explicar a implementação do **CFS** e como ele difere dos demais escalonadores.

---

### Implementação do CFS

#### Estrutura de Dados

O CFS foi implementado utilizando uma **árvore rubro-negra (Red-Black Tree)**, que garante:

- Inserção, remoção e busca em **tempo O(log n)**.
- Ordenação automática dos threads com base em **virtual runtime (vruntime)**, que é o tempo de CPU ajustado pela prioridade.
- Suporte a desempates complexos para threads com mesmo vruntime, considerando:
  1. Tempo de início do processo (`start_time`).
  2. PID do processo.
  3. Endereço do TCB para garantir ordem total.

#### Funcionamento

- Cada thread é representada por um `TCB` (Thread Control Block).
- Threads terminadas (`tcb_is_terminated`) são ignoradas na seleção.
- A função `CFS_min` percorre a árvore e retorna a thread com menor vruntime ativa.
- Threads são inseridas na árvore através de `RBT_insert`.

#### Diferença em relação aos demais escalonadores

| Escalonador | Estrutura | Seleção da próxima thread | Preempção |
|-------------|-----------|--------------------------|-----------|
| FCFS        | FIFO      | Primeiro a entrar        | Não       |
| RR          | FIFO      | Próximo na fila          | Sim       |
| Priority    | Heap      | Thread com maior prioridade | Sim     |
| CFS         | Red-Black Tree | Thread com menor vruntime | Sim, de forma justa |

**Observações:**

- Ao contrário do RR, que apenas percorre a fila circularmente, o CFS tenta equilibrar o tempo de CPU entre todas as threads, garantindo **justiça**.
- Diferente do Priority, que pode sofrer de starvation para threads de baixa prioridade, o CFS utiliza o `vruntime` para que todos os processos recebam tempo proporcional à sua prioridade.
- Threads terminadas são descartadas automaticamente, mas a árvore mantém a ordem das threads ativas.

---

## Resumo do fluxo CFS

1. Threads são criadas a partir de processos e adicionadas na árvore rubro-negra.
2. A cada quantum ou evento de escalonamento:
   - `CFS_min` seleciona a thread com menor vruntime.
   - A thread executa por seu quantum.
   - Após a execução:
     - Se terminou, é marcada como finalizada na árvore.
     - Se não terminou, é reinserida na árvore com vruntime atualizado.
3. O kernel continua o ciclo até todas as threads terminarem.

---

## Conclusão

O CFS implementado neste mini-kernel garante **fairness** entre processos, aproveitando a eficiência e propriedades da Red-Black Tree para manter a complexidade logarítmica das operações. Ele se diferencia dos outros escalonadores por balancear o tempo de CPU proporcional à prioridade e ao tempo de execução de cada thread.
