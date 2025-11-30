import java.time.LocalDate;

public final class Agendamento {
    private LocalDate data;
    private String status;
    //TODOs: Aqui tem que ser uma lista de produtos

    public void confirmarPedido(){

    }

    public void remarcarPedido(){

    }

    public String getStatus(){
        return this.status;
    }

    public void statusEmAnalise(){
        this.status = "Pedido em análise";
    }

    public void statusAceito(){
        this.status = "Pedido aceito";
    }

    public void statusFinalizado(){
        this.status = "Pedido finalizado";
    }

    public LocalDate getData(){
        return this.data;
    }

    public void setData(LocalDate data){
        this.data = data;
    }

    //TODOs: Para criar um agendamento deve se ter no minimo um pedido e uma data
    public Agendamento(LocalDate data){
        // Inserir a lista de pedidos do cliente na variavel "produtos"
        statusEmAnalise();
        setData(data);
    }
}
