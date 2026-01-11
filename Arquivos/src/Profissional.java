import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class Profissional extends Pessoa {
    private List<Pedido> pedidos;
    private String especialidade;
    private int id;
    private double valorMinimoPedido;
    private int tempoExperiencia; // em anos
    private AgendaSemanal agenda;
    private static int qtdProfissionais = 0;

    private Thread workerThread;
    private volatile Pedido pedidoAtual;
    private volatile long inicioPreparoMs; // início do preparo atual (ms)
    private volatile long totalPreparoMs; // total ms do preparo atual
    // escala temporal: 1 hora do sistema = 15 segundos reais
    private static final long MS_POR_HORA = 15000L;

    Notificacao notf = new Notificacao();

    public Pedido getPedidoAtual() {
        return pedidoAtual;
    }

    public List<Pedido> getPedidos() {
        return pedidos;
    }

    public void setPedidos(List<Pedido> pedidos) {
        this.pedidos = pedidos;
    }

    public void setEspecialidade(String especialidade) {
        this.especialidade = especialidade;
    }

    public String getEspecialidade() {
        return especialidade;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setValorMinimoPedido(double valorMinimoPedido) {
        this.valorMinimoPedido = valorMinimoPedido;
    }

    public double getValorMinimoPedido() {
        return valorMinimoPedido;
    }

    public void setTempoExperiencia(int tempoExperiencia) {
        this.tempoExperiencia = tempoExperiencia;
    }

    public int getTempoExperiencia() {
        return tempoExperiencia;
    }

    public void setAgenda() {
        this.agenda = new AgendaSemanal();
    }

    public static void incrementaQtdProfissionais(){
        qtdProfissionais++;
    }

    public Profissional(String cpf, String nome, String email, String telefone, LocalDate dataNascimento,
            String especialidade, double valorMinimoPedido, int tempoExperiencia) {
        super(cpf, nome, email, telefone, dataNascimento);
        setEspecialidade(especialidade);
        setId(qtdProfissionais);
        incrementaQtdProfissionais();

        setValorMinimoPedido(valorMinimoPedido);
        setTempoExperiencia(tempoExperiencia);
        setPedidos(new ArrayList<>());
        setAgenda();
        iniciarThreadWorker();
    }

    // inicia a thread do profissional que processa a fila de pedidos
    private void iniciarThreadWorker() {
        workerThread = new Thread(new Runnable() {
            public void run() {
                while (true) {
                    Pedido prox = null;
                    synchronized (pedidos) {
                        while (pedidos.isEmpty()) {
                            try {
                                pedidos.wait();
                            } 
                            catch (InterruptedException e) {
                                // se a thread for interrompida enquanto espera, apenas continua
                            }
                        }
                        prox = pedidos.remove(0);
                    }

                    pedidoAtual = prox;
                    inicioPreparoMs = System.currentTimeMillis();
                    // tempo de preparo proporcional à duração: duracao (horas) * MS_POR_HORA
                    totalPreparoMs = (long) pedidoAtual.getDuracao() * MS_POR_HORA;

                    prox.setStatus("Em preparo");
                    try {
                        if (totalPreparoMs > 0) {
                            Thread.sleep(totalPreparoMs);
                        }
                        
                        prox.setStatus("Pedido finalizado");
                        // log simples: fim do preparo
                        notf.notificacaoPadrao();
                        System.out.println("Pedido ID " + prox.getId() + " finalizado pelo profissional: " + getNome());
                        System.out.println();
                    } 
                    catch (InterruptedException e) {
                        // pedido foi cancelado durante preparo
                        prox.setStatus("Cancelado");
                        
                        // limpa o flag de interrupção
                        Thread.interrupted();
                        
                        // mensagem de cancelamento
                        notf.notificacaoPadrao();
                        System.out.println("Pedido ID " + prox.getId() + " cancelado.");
                        System.out.println();
                    } 
                    finally {
                        pedidoAtual = null;
                        inicioPreparoMs = 0;
                        totalPreparoMs = 0;
                    }
                }
            }
        });
        workerThread.setDaemon(true);
        workerThread.start();
    }

    public void imprimeInformacoesProfissional() {
        System.out.println("Nome: " + getNome());
        System.out.println("Especialidade: " + getEspecialidade());
        System.out.println("ID: " + getId());
        System.out.println("Valor Minimo do Pedido: " + getValorMinimoPedido());
        agenda.imprimeInformacoesAgenda();
        System.out.println();
    }

    public boolean confirmarPedidoProfissional(Pedido pedido) {
        if (confirmarDiaProfissional(pedido.getDuracao())) {
            synchronized (pedidos) {
                this.pedidos.add(pedido);
                pedidos.notify();
            }
            // notificar o usuário que o pedido foi adicionado à fila
            notf.notificacaoPadrao();
            System.out.println("Pedido ID " + pedido.getId() + " adicionado a fila.");
            System.out.println();
            return true;
        }
        return false;
    }

    public int HorasDisponiveis() {
        return this.agenda.HorasDisponiveisAgenda();
    }

    private boolean confirmarDiaProfissional(int horasServico) {
        if (agenda.marcarHorarioProfissional(horasServico)) {
            return true;
        }
        return false;
    }

    // cancela pedido por id e cpf do cliente; devolve horas de forma proporcional
    // ao tempo restante
    // retorna: 0 = não encontrado / erro, 1 = cancelado durante preparo, 2 =
    // cancelado na fila
    public int cancelarPedidoPorId(int pedidoId, String clienteCpf) {
        // primeiro, confere se o pedido em preparo é o pedido alvo
        if (pedidoAtual != null) {
            if (pedidoAtual.getId() == pedidoId) {
                if (clienteCpf != null) {
                    if (clienteCpf.equals(pedidoAtual.getClienteCpf())) {
                        notf.notificacaoPadrao();
                        System.out.println("Seu pedido ja esta em preparo, logo, sera cobrada uma taxa de 50% do valor total.");
                        System.out.println("Valor a ser cobrado " + pedidoAtual.getValor() / 2);
                        
                        // calcula tempo restante antes de interromper
                        long totalMs = totalPreparoMs;
                        long elapsed = System.currentTimeMillis() - inicioPreparoMs;
                        long remainingMs = totalMs - elapsed;
                        
                        if (remainingMs < 0) {
                            remainingMs = 0;
                        }
                        
                        int duracaoHoras = pedidoAtual.getDuracao();
                        int horasADevolver = 0;
                        
                        if (totalMs > 0) {
                            double frac = (double) remainingMs / (double) totalMs;
                            horasADevolver = (int) Math.floor(duracaoHoras * frac);
                        } 
                        else {
                            horasADevolver = 0;
                        }

                        // devolve horas proporcionais
                        if (horasADevolver > 0) {
                            agenda.liberarHoras(horasADevolver);
                        }

                        // imprimir mensagem de cancelamento (sem citar o profissional)
                        System.out.println("Pedido ID " + pedidoAtual.getId() + " cancelado.");
                        System.out.println();
                        
                        // interrompe a thread worker para cancelar o pedido em preparo
                        workerThread.interrupt();
                        return 1;
                    } 
                    else {
                        return 0;
                    }
                } 
                else {
                    return 0;
                }
            }
        }

        // se não é o pedido em preparo, procura na fila
        synchronized (pedidos) {
            for (int i = 0; i < pedidos.size(); i++) {
                Pedido p = pedidos.get(i);
                if (p.getId() == pedidoId) {
                    if (clienteCpf != null) {
                        if (clienteCpf.equals(p.getClienteCpf())) {
                            p.setStatus("Cancelado");
                            pedidos.remove(i);
                            
                            // pedido não iniciado -> remaining == total -> devolve duracao inteira
                            int dur = p.getDuracao();
                            int horasADevolver = dur; // floor(dur * 1) == dur
                            if (horasADevolver > 0) {
                                agenda.liberarHoras(horasADevolver);
                            }
                            
                            // notificar cliente / registrar cancelamento sem penalidade
                            notf.notificacaoPadrao();
                            System.out.println("Pedido ID " + p.getId() + " cancelado.");
                            System.out.println();
                            return 2;
                        } 
                        else {
                            return 0;
                        }
                    } 
                    else {
                        return 0;
                    }
                }
            }
        }

        return 0;
    }

}
