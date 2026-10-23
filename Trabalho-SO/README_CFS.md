-> README - Escalonador CFS (Completely Fair Scheduler) <-

-> Visão Geral
    |
     -> O CFS (Completely Fair Scheduler) é um dos algoritmos de escalonamento implementados neste sistema. 
    Ele utiliza uma árvore rubro-negra para gerenciar os processos prontos de forma eficiente, garantindo 
    justiça na alocação de tempo de CPU com base no vruntime de cada thread.

-> Estrutura de Dados
    |
     -> Árvore Rubro-Negra (Red-Black Tree)
    |
     -> A estrutura principal para o CFS é uma árvore rubro-nega que armazena TCBs:

                        typedef struct TRBTree {
                            TNoRB *raiz;
                            int sz;
                            int (*cmp)(const void *, const void *);
                        } TRBTree;
 
-> Funcionamento do CFS
    |
    |-> 1. Inicialização
    |   |
    |    -> O CFS é selecionado como tipo de escalonador (Type 4)
    |   |
    |    -> Uma árvore rubro-negra é criada com função de comparação personalizada
    |
    |
     -> 2. Função de Comparação
    |   |
    |    -> A ordenação é feita com base em múltiplos critérios:
    |
    |               static int compare_cfs_tcb(const void *a, const void *b) {
    |                   // 1. Compara vruntime (menor vruntime tem prioridade)
    |                   // 2. Desempate: tempo de início do processo
    |                   // 3. Desempate: PID do processo
    |                   // 4. Desempate final: endereço do TCB (garante ordem total)
    |               }
    |
    |
     -> 3. Inserção de Threads
    |   |
    |    -> Cada processo é dividido em múltiplas threads
    |   |
    |    -> Cada thread é inserida na árvore rubro-negra:
    |
    |          void RBT_insert(TRBTree *tree, void *key);
    |
    |
     -> 4. Seleção para Execução
    |   |
    |    -> O escalonador sempre seleciona a thread com menor vruntime:
    |
    |           TCB *cfs_scheduler(TRBTree *tree) {
    |               return CFS_min(tree); // Retorna o nó mais à esquerda
    |           }
    |
    |
     -> 5. Busca do Mínimo com Filtragem
    |   |
    |    ->A função CFS_min() percorre a árvore procurando o menor vruntime entre threads não finalizadas:
    |
    |               static TCB *cfs_min_node(TRBTree *tree, TNoRB *h) {
    |                   // Ignora threads terminadas (tcb_is_terminated(cur))
    |                   // Compara recursivamente subárvores esquerda, direita e atual
    |                   // Retorna o TCB com menor vruntime ativo
    |               }
    |
     -> 6. Execução e Atualização
    |   |
    |    -> A thread selecionada é executada na CPU
    |   |
    |    -> O vruntime é atualizado durante a execução:
    |
    |               void tcb_working_thread(TCB *tcb, double quantum) {
    |                   // Calcula tempo executado
    |                   // Atualiza vruntime: vruntime += (tempo_executado / peso_da_prioridade)
    |                   // Peso baseado na prioridade: 1.0 + (40 - priority) * 0.1
    |               }
    |
     -> 7. Reinserção na Árvore
        |
         -> Após executar, a thread é reinserida na árvore (não removida)
        |
         -> Isso mantém a estrutura da árvore intacta
        |
         -> O vruntime atualizado garante nova posição na árvore

-> Gerenciamento de Término
    |
     -> Problema Identificado
    |   |
    |    -> Não é possível remover nós da árvore sem causar segmentation fault
    |   |
    |    -> Múltiplas threads apontam para o mesmo BCP
    |
    |
    -> Solução Implementada
        |
         -> Marcação de threads finalizadas: tcb_set_terminated(tcb)
        |
         -> Filtragem na busca: CFS_min() ignora threads terminadas
        |
         -> Estrutura auxiliar: Lista no kernel para armazenar threads finalizadas
        |
         -> Limpeza final: Destruição controlada ao final da simulação

                        // No kernel.h
                        TCB **finished;  // Array de threads finalizadas
                        int n_finished;   // Contador


-> API Principal
    |
     -> Funções da Árvore Rubro-Negra
    |   |
    |    -> TRBTree *RBT_create(int (*cmp)(const void *, const void *));
    |   |
    |    -> void RBT_destroy(TRBTree *tree);
    |   |
    |    -> void RBT_insert(TRBTree *tree, void *key);
    |   |
    |    -> void *RBT_search(TRBTree *tree, void *key);
    |   |
    |    -> void *CFS_min(TRBTree *tree);
    |   |
    |    -> int RBT_is_empty(TRBTree *tree);
    |   |
    |    -> int RBT_size(TRBTree *tree);
    |
    |
     -> Operações de Balanceamento
        |
         -> static TNoRB *rotLeft(TNoRB *h);     // Rotação à esquerda
        |
         -> static TNoRB *rotRight(TNoRB *h);    // Rotação à direita
        |
         -> static void flipColors(TNoRB *h);    // Inversão de cores


->Vantagens do Design
    |
     -> Eficiência: Busca do mínimo em O(log n)
    |
     -> Justiça: Sempre seleciona a thread mais "atrasada" (menor vruntime)
    |
     ->Estabilidade: Reinserção mantém a árvore balanceada
    |
     ->Controle de recursos: Gerenciamento adequado de threads finalizadas

-> Considerações Importantes
    |
     -> A árvore nunca remove nós durante a execução
    |
     -> Threads finalizadas são ignoradas na busca por mínimo
    |
     -> A limpeza de memória ocorre apenas no final da simulação
    |
     -> O vruntime é a métrica fundamental para justiça do escalonamento

--------------------------------------------------------------------------------------------------
|  Este design garante que o CFS opere de forma eficiente e justa, distribuindo o tempo de CPU   |
|  de maneira equitativa entre todas as threads ativas do sistema.                               |
--------------------------------------------------------------------------------------------------      