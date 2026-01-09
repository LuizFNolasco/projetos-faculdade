# 🍰 Confeitaria Doce Encanto - Sistema de Gerenciamento

## 📖 Sobre o Projeto

O **Sistema Doce Encanto** é uma solução robusta baseada em Java para o gerenciamento integral de uma confeitaria. O projeto aplica conceitos avançados de Orientação a Objetos e Concorrência para automatizar processos de negócios, desde a contratação de talentos até a entrega final do pedido.

O sistema se destaca pela **simulação temporal**, onde o tempo de preparo dos produtos é processado em tempo real (escala reduzida) utilizando *Threads*, e pela lógica de alocação dinâmica de profissionais baseada em especialidade e disponibilidade.

---

## 🚀 Funcionalidades Principais

### 👥 Gestão de Pessoas e Hierarquia
* **Herança e Polimorfismo:** Todas as entidades (`Cliente`, `Profissional`, `Candidato`, `Administrador`) herdam da classe base abstrata `Pessoa`, garantindo consistência nos dados cadastrais.
* **RH Automatizado:** O `Administrador` avalia `Candidatos` com base em critérios rígidos (Experiência > 5 anos, Pretensão Salarial < R$ 3.000,00).

### 🧁 Catálogo e Pedidos
* **Produtos Especializados:** Cadastro de itens (`Produto`) com atributos de tempo de preparo e especialidade necessária (ex: "Bolo" requer um "Confeiteiro").
* **Simulação de Preparo (Threads):** Cada `Profissional` possui uma *Thread* dedicada (`workerThread`) que processa a fila de pedidos.
    * *Escala:* 1 hora de preparo no sistema equivale a 15 segundos reais.
    * O sistema notifica automaticamente quando um pedido é finalizado.

### 📅 Agenda e Alocação Inteligente
* **Algoritmo de Alocação:** Ao receber um `Agendamento`, o sistema busca automaticamente um profissional que possua a especialidade exigida E horas disponíveis na sua `AgendaSemanal`.
* **Gestão de Horas:** Controle granular de horas diárias (máximo de 8h/dia). As horas são debitadas na aceitação e estornadas (total ou parcialmente) em caso de cancelamento.

### 💰 Financeiro e Regras de Negócio
* **Política de Cancelamento:**
    * Cancelamentos durante o preparo geram multa de 50% do valor.
    * As horas de trabalho são devolvidas proporcionalmente ao tempo restante do preparo interrompido.
* **Controle de Dívidas:** Clientes acumulam dívidas em caso de não pagamento ou multas, bloqueando novos pedidos dependendo da regra aplicada.

---

## 🛠️ Arquitetura do Sistema

O projeto segue uma arquitetura modular focada em objetos de domínio. Abaixo, o diagrama de classes ilustrando as principais relações:

```mermaid
classDiagram
    class Pessoa {
        <<Abstract>>
        +String nome
        +String cpf
        +String email
    }
    class Cliente {
        -double divida
        -List~Agendamento~ agendamentos
    }
    class Profissional {
        -List~Pedido~ pedidos
        -AgendaSemanal agenda
        -Thread workerThread
        +iniciarThreadWorker()
    }
    class Agendamento {
        -List~Produto~ produtos
        +solicitacaoDeProfissional()
    }
    class AgendaSemanal {
        -Dia[] dias
        +marcarHorarioProfissional()
    }
    class Produto {
        +String especialidadeNecessaria
        +int tempoDePreparo
    }
    
    Pessoa <|-- Cliente
    Pessoa <|-- Profissional
    Pessoa <|-- Candidato
    Pessoa <|-- Administrador
    
    Cliente "1" --> "*" Agendamento : realiza
    Agendamento "1" --> "*" Produto : contem
    Profissional "1" *-- "1" AgendaSemanal : gerencia
    Profissional "1" o-- "*" Pedido : processa