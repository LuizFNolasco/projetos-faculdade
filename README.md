# Work & Rest

Aplicação web para gestão de tempo baseada na técnica Pomodoro, voltada principalmente ao público estudantil, mas útil a qualquer pessoa que busque produtividade em atividades que exigem concentração.

## Propósito

O sistema auxilia na gestão de tempo em relação a atividades de estudo e/ou trabalho, buscando maximizar a eficiência dessas tarefas por meio de ciclos de foco intercalados com pausas.

## Usuários

O público-alvo é majoritariamente estudantil, embora pessoas de outras áreas também possam utilizar a plataforma. A plataforma suporta contas de usuários (com login/senha) onde todo o histórico de logs é salvo na nuvem, mas também possibilita um uso rápido anônimo ("Começar sem conta").

## Funcionalidades

### Timer Pomodoro Global e Persistente

- O cronômetro é gerenciado por uma engine global de estados. Você pode iniciar um ciclo de estudo, navegar livremente pelas outras telas (Logs, Dados, Configurações) e o cronômetro continuará operando em segundo plano.
- Timer automático baseado nos ciclos da técnica Pomodoro, avançando automaticamente entre os estados.
- Presets prontos, como o **Pomodoro clássico** (4 ciclos — 25 min de foco / 5 min de pausa curta — pausa longa de 15 min).

### Personalização

- Configuração customizada de tempos de foco, pausa curta, pausa longa e quantidade de ciclos até a pausa longa.

### Dados e logs de estudo

- Cada sessão de estudo registra horário de início/término e ciclos completados.
- Gráficos diários, semanais e mensais (calculados agregando o histórico de uso no banco de dados).
- O histórico de sessões é salvo em um banco de dados relacional para usuários cadastrados, de onde podem ser consultados e até mesmo excluídos pela interface.

## Tecnologias

| Camada        | Tecnologia                          |
|---------------|-------------------------------------|
| Front-end     | React 19 + Next.js 16 (App Router)  |
| Back-end      | API Routes / Server Actions (Next.js) |
| Banco de Dados| PostgreSQL + Prisma ORM             |
| Linguagem     | TypeScript                          |
| Estilização   | Tailwind CSS 4                      |
| Componentes   | shadcn/ui + Base UI                 |
| Gerenciador   | pnpm                                |

---

## Documentação

### Diagrama de classes do domínio do problema

O diagrama abaixo representa as entidades centrais do domínio persistidas e mapeadas via Prisma. As sessões de estudo são a **fonte da verdade** dos dados. O resumo diário e os gráficos são derivados por agregação no frontend ou via queries agregadas, sem entidade persistida à parte para gráficos.

```mermaid
classDiagram
    direction TB

    class Usuario {
        +String id
        +String nome
        +String email
        +String senha
        +DateTime criadoEm
        +autenticar(email: String, senha: String) boolean
        +iniciarSessao(config: ConfiguracaoTimer) SessaoEstudo
        +obterSessoes(periodo: Periodo) List~SessaoEstudo~
        +obterResumoPorDia(data: Date) ResumoDiario
        +obterGrafico(periodo: Periodo) GraficoEstudo
    }

    class ConfiguracaoTimer {
        +String id
        +String nome
        +int duracaoFocoMin
        +int duracaoPausaCurtaMin
        +int duracaoPausaLongaMin
        +int ciclosAtePausaLonga
        +boolean ehPreset
        +validar() boolean
    }

    class PresetTimer {
        <<enumeration>>
        CLASSICO
        CUSTOMIZADO
    }

    class SessaoEstudo {
        +String id
        +String usuarioId
        +DateTime inicio
        +DateTime? fim
        +int ciclosCompletos
        +EstadoTimer estado
        +registrarCiclo() void
        +finalizar() void
    }

    class ResumoDiario {
        <<valor calculado>>
        +Date data
        +int totalCiclos
        +int tempoTotalMin
        +List~SessaoEstudo~ sessoes
    }

    class EstadoTimer {
        +FaseTimer faseAtual
        +int cicloAtual
        +int tempoRestanteSeg
        +ConfiguracaoTimer configuracao
        +avancarFase() void
        +reiniciar() void
    }

    class FaseTimer {
        <<enumeration>>
        FOCO
        PAUSA_CURTA
        PAUSA_LONGA
    }

    class GraficoEstudo {
        <<valor calculado>>
        +String usuarioId
        +Periodo periodo
        +List~PontoGrafico~ pontos
        +gerar(sessoes: List~SessaoEstudo~) GraficoEstudo
    }

    class Periodo {
        +Date inicio
        +Date fim
    }

    class PontoGrafico {
        +Date data
        +int ciclos
        +int minutosEstudados
    }

    Usuario "1" --> "*" SessaoEstudo : realiza
    Usuario "1" --> "*" ConfiguracaoTimer : possui
    ConfiguracaoTimer ..> PresetTimer : tipo
    SessaoEstudo "1" --> "1" EstadoTimer : mantém
    SessaoEstudo "*" --> "1" ConfiguracaoTimer : utiliza
    EstadoTimer "1" --> "1" FaseTimer : faseAtual
    Usuario ..> ResumoDiario : agrega sessões por dia
    ResumoDiario ..> SessaoEstudo : compõe
    Usuario ..> GraficoEstudo : visualiza
    GraficoEstudo ..> SessaoEstudo : agrega por data
    GraficoEstudo "1" --> "*" PontoGrafico : exibe
    GraficoEstudo "1" --> "1" Periodo : cobre
```

**Resumo das entidades persistidas:**

| Entidade             | Tipo        | Responsabilidade                                              |
|----------------------|-------------|---------------------------------------------------------------|
| `Usuario`            | Entidade    | Identifica quem usa a plataforma; credenciais (`email`, `senha`) e acesso a sessões e histórico |
| `ConfiguracaoTimer`  | Entidade    | Define durações e ciclos do Pomodoro (preset ou customizado)  |
| `SessaoEstudo`       | Entidade    | Fonte da verdade: registra início, fim e ciclos de cada sessão |

---

### Ferramentas escolhidas

| Categoria        | Ferramenta              | Uso no projeto                                              |
|------------------|-------------------------|-------------------------------------------------------------|
| Versionamento    | **Git** + **GitHub**    | Controle de versão, branches e histórico de alterações      |
| Build            | **Next.js** + **pnpm**  | Compilação, bundling e scripts de desenvolvimento/produção  |
| Banco de Dados   | **PostgreSQL** + **Prisma** | Armazenamento de dados relacionais e modelagem de acesso   |
| Testes           | **Vitest** + **Testing Library** | Testes unitários e de componentes React              |
| Lint / qualidade | **ESLint**              | Análise estática e padronização de código                   |
| Issue tracking   | **Trello**       | Registro de bugs, tarefas e acompanhamento do backlog        |
| CI/CD            | **GitHub Actions**      | Pipeline automatizado de lint, testes e build               |
| Container        | **Docker**              | Execução do banco de dados (postgres) via compose              |

---

### Frameworks reutilizados

| Framework / biblioteca | Finalidade                                              |
|------------------------|---------------------------------------------------------|
| [Next.js](https://nextjs.org/) | Framework full-stack React (rotas, SSR, API Routes) |
| [React](https://react.dev/)    | Construção da interface de usuário                    |
| [Prisma](https://www.prisma.io/)| ORM moderno e type-safe para comunicação com PostgreSQL|
| [Tailwind CSS](https://tailwindcss.com/) | Estilização utilitária e responsiva         |
| [shadcn/ui](https://ui.shadcn.com/) | Componentes acessíveis e customizáveis          |
| [Lucide React](https://lucide.dev/) | Ícones                                              |
| [TypeScript](https://www.typescriptlang.org/) | Tipagem estática em todo o código-fonte   |

---

### Como gerar a documentação do código

Em projetos TypeScript, o equivalente ao JavaDoc é o **[TypeDoc](https://typedoc.org/)**, que gera documentação HTML a partir de comentários **TSDoc** no código.

```bash
# Executar a documentação (se configurado no package.json)
pnpm docs
```

---

### Como executar o sistema

#### Pré-requisitos

- [Node.js](https://nodejs.org/) 20 ou superior
- [pnpm](https://pnpm.io/) 9 ou superior
- Instância do PostgreSQL rodando localmente ou via Docker (veja o compose).

#### 1. Clonar o repositório

```bash
git clone <url-do-repositorio>
cd work-and-rest
```

#### 2. Instalar as dependências

```bash
pnpm install
```

#### 3. Subir o Banco de Dados com Docker

Para iniciar a instância do PostgreSQL localmente, utilize o Docker Compose:

```bash
docker compose up -d
```

O banco rodará em segundo plano na porta 5432.

#### 4. Configurar Variáveis de Ambiente

Crie um arquivo `.env` na raiz do projeto contendo a sua string de conexão:

```env
DATABASE_URL="postgresql://usuario:senha@localhost:5432/workandrest?schema=public"
```

#### 5. Rodar Migrations e Gerar o Prisma Client

```bash
npx prisma db push
npx prisma generate
```

> **Aviso Importante**: O Prisma Client deste projeto foi configurado com um output customizado no caminho `lib/generated/prisma`. Portanto, sempre importe o Prisma dessa pasta.

#### 6. Executar em modo de desenvolvimento

```bash
pnpm dev
```

Acesse [http://localhost:3000](http://localhost:3000) no navegador.

---

## Estrutura do projeto

```
work-and-rest/
├── app/                  # Rotas e páginas (App Router do Next.js)
│   ├── layout.tsx        # Layout raiz e contexto
│   ├── page.tsx          # Página inicial e Landing Page
│   ├── timer/            # Tela principal de Foco
│   ├── logs/             # Histórico
│   └── globals.css       # Estilos globais
├── components/           # Componentes React reutilizáveis
│   ├── foco/             # Lógica visual e controles do cronômetro
│   ├── providers/        # Provedores de Contexto Globais (Timer e Configs)
│   └── ui/               # Componentes shadcn/ui base
├── lib/                  # Utilitários, cálculo de sessão, Prisma DB
├── prisma/               # Schema do Banco de Dados PostgreSQL
├── public/               # Arquivos estáticos
├── package.json
├── tsconfig.json
└── README.md
```

## Licença

Projeto acadêmico desenvolvido no contexto da disciplina de Projeto Integrado (PI).
