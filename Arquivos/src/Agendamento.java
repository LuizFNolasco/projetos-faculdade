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

    /*
        Essa função vai criar o pedido e atribuir ao profissional

        Para cada Produto deve-se avaliar se há algum profissional com essa especialidade disponivel 
        Primeiro -> verifica se o profissional trabalha com esse produto
    */

    //TODOs: Verificar se isso ta certo (ESTOU COM MUITO SONO PRA TESTAR AGORA SORRY)
    public void solicitacaoDeProfissional(Profissional[] profissionais){
        List<Produto> produtosAux = this.produtos;

        for(Profissional profissional : profissionais){
            int cargaRestante = profissional.HorasDisponiveis();
            //TODOs: Aqui a gente vai usar conferirHorarioProfissional e so depois confirma o pedido atribuindo ao profissional
            for (int i = 0; i < produtosAux.size(); i++) {
                Produto produto = produtosAux.get(i);

                if (profissional.getEspecialidade() == produto.getEspecialidadeNecessaria()) {
                    int tempoNecessario = produto.getTempoDePreparo();

                    if (tempoNecessario <= cargaRestante) {
                        cargaRestante -= tempoNecessario;
                        produtosAux.remove(i);
                        i--; // ajusta o índice após remoção
                    }
                }
            }
        }
        // Após tentar alocar profissionais para todos os produtos:
        if (produtosAux.isEmpty()) {
            // todos os produtos receberam profissional → pedido aceito
            statusAceito();
            //TODOs: Criação de um pedido pra cada produto.
        } else {
            this.status = "Pedido recusado";
        }
    }
}

/*
//confere se o profissional trabalha com o tipo de servico solicitado
            String mensagem = ("Profissional disponivel: "+profissional.getNome()+"\nEspecialidade: "+profissional.getEspecialidade()+"\n");
            int confirmacao = notificacao.notificacaoEspecifica(mensagem);
            // else, notifica indisponibilidade
            // trata os casos de confirmacao ou recusa do profissional
*/
