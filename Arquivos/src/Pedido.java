import java.time.LocalDate;

public final class Pedido {
    private LocalDate data;
    private int duracao;
    private double valor;
    private String status; // TODOs: Precisa de outra variavel status? A do agendamento não bastaria?
    private int quantidade;

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

    //TODOs: Realmente tem que ter esse metodo aqui? Acho que o do Agendamento já bastaria
    public void aceitarPedido(){

    }

    public Pedido(LocalDate data, int duracao, double valor, String status, int quantidade){
        setData(data);
        setDuracao(duracao);
        setValor(valor);
        setStatus("Pedido em análise");
        setQuantidade(quantidade);
    }
}
