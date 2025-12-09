import java.time.LocalDate;
import java.util.List;

public final class Agendamento {
    private LocalDate data;
    private String status;
    private List<Produto> produtos;
    private Notificacao notificacao;

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

    public List<Produto> getProdutos() {
        return produtos;
    }

    public void setProdutos(List<Produto> produtos) {
        this.produtos = produtos;
    }

    //Alteração: Adicionei uma lista de produtos como parametros
    public Agendamento(LocalDate data, Notificacao notificacao, List<Produto> produtos){
        statusEmAnalise();
        setData(data);
        setProdutos(produtos);
        this.notificacao = notificacao;
    }

    public void solicitacaoDeProfissional(Profissional[] profissionais){
        for(Profissional profissional : profissionais){
            //confere se o profissional trabalha com o tipo de servico solicitado
            Dia prazo = profissional.conferirAgendaProfissional(4);
            if(prazo != null){
                String mensagem = ("Profissional disponivel: "+profissional.getNome()+"\nEspecialidade: "+profissional.getEspecialidade()+"\nDia: "+prazo.getData());
                int confirmacao = notificacao.notificacaoEspecifica(mensagem);
            }
            // else, notifica indisponibilidade
            // trata os casos de confirmacao ou recusa do profissional
        }
    }
}
