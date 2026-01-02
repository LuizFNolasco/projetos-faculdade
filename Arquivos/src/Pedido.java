import java.time.LocalDate;

public final class Pedido {

    private static int nextId = 1;

    private int id;
    private LocalDate data;
    private int duracao;
    private double valor;
    private Produto produto;
    private String status; // Status: em preparo, concluido, cancelado
    private int quantidade;
    private String clienteCpf;

    public int getId() {
        return id;
    }

    public String getClienteCpf() {
        return clienteCpf;
    }

    public void setClienteCpf(String clienteCpf) {
        this.clienteCpf = clienteCpf;
    }

    public Produto getProduto() {
        return produto;
    }

    public void setProduto(Produto produto) {
        this.produto = produto;
    }

    public LocalDate getData() {
        return data;
    }

    public void setData(LocalDate data) {
        this.data = data;
    }

    public int getDuracao() {
        return duracao;
    }

    public void setDuracao(int duracao) {
        this.duracao = duracao;
    }

    public double getValor() {
        return valor;
    }

    public void setValor(double valor) {
        this.valor = valor;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public int getQuantidade() {
        return quantidade;
    }

    public void setQuantidade(int quantidade) {
        this.quantidade = quantidade;
    }

    // Altered constructor: adds clienteCpf and assigns unique id
    public Pedido(LocalDate data, int duracao, double valor, String status, int quantidade, String clienteCpf) {
        this.id = nextId++;
        setData(data);
        setDuracao(duracao);
        setValor(valor);
        setStatus(status);
        setQuantidade(quantidade);
        setClienteCpf(clienteCpf);
    }
}
