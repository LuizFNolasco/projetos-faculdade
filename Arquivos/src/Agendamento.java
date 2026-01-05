import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Agendamento {
    private LocalDate data;
    private String status;
    private List<Produto> produtos;

    public String getStatus() {
        return this.status;
    }

    public void statusEmAnalise() {
        this.status = "Pedido em análise";
    }

    public void statusAceito() {
        this.status = "Pedido aceito";
    }

    public void statusFinalizado() {
        this.status = "Pedido finalizado";
    }

    public LocalDate getData() {
        return this.data;
    }

    public void setData(LocalDate data) {
        this.data = data;
    }

    public List<Produto> getProdutos() {
        return produtos;
    }

    public void setProdutos(List<Produto> produtos) {
        this.produtos = produtos;
    }

    // Alteração: Adicionei uma lista de produtos como parametros
    public Agendamento(LocalDate data, List<Produto> produtos) {
        statusEmAnalise();
        setData(data);
        setProdutos(produtos);
    }

    // agora recebe o cliente solicitante para vincular o pedido
    public void solicitacaoDeProfissional(Profissional[] profissionais, Cliente cliente) {
        Map<Produto, Profissional> alocacaoSimulada = new HashMap<>();
        Map<Profissional, Integer> horasRestantes = new HashMap<>();

        for (int i = 0; i < profissionais.length; i++) {
            Profissional p = profissionais[i];
            horasRestantes.put(p, p.HorasDisponiveis());
        }

        for (int i = 0; i < produtos.size(); i++) {
            Produto prod = produtos.get(i);
            boolean encontrado = false;

            for (int j = 0; j < profissionais.length; j++) {
                Profissional p = profissionais[j];
                // comparar conteúdo das Strings (null-safe)
                if (p.getEspecialidade() != null && p.getEspecialidade().equals(prod.getEspecialidadeNecessaria())) {
                    int tempo = prod.getTempoDePreparo();

                    if (horasRestantes.get(p) >= tempo) {
                        horasRestantes.put(p, horasRestantes.get(p) - tempo);
                        alocacaoSimulada.put(prod, p);
                        encontrado = true;
                        break;
                    }
                }
            }

            if (!encontrado) {
                this.status = "Rejeitado";
                return;
            }
        }

        for (int i = 0; i < produtos.size(); i++) {
            Produto prod = produtos.get(i);
            Profissional p = alocacaoSimulada.get(prod);
            System.out.println("Profissional Solicitado:");
            p.imprimeInformacoesProfissional();

            String clienteCpfParaPedido;
            if (cliente != null) {
                clienteCpfParaPedido = cliente.getCpf();
            } else {
                clienteCpfParaPedido = null;
            }

            Pedido pedido = new Pedido(
                    this.data,
                    prod.getTempoDePreparo(),
                    prod.getValor(),
                    "Em preparo",
                    1,
                    clienteCpfParaPedido);

            pedido.setProduto(prod);

            boolean ok = p.confirmarPedidoProfissional(pedido);

            if (!ok) {
                this.status = "Rejeitado";
                return;
            }
        }

        this.statusAceito();

    }
}