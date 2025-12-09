import java.time.LocalDate;

public final class Pedido {

    private LocalDate data;
    private int duracao;
    private double valor;
    private Produto produto; 
    private String status; //Status: em preparo, concluido, cancelado
    private int quantidade;


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

    public Pedido(LocalDate data, int duracao, double valor, String status, int quantidade){
        setData(data);
        setDuracao(duracao);
        setValor(valor);
        setStatus("Pedido em análise");
        setQuantidade(quantidade);
    }
}
