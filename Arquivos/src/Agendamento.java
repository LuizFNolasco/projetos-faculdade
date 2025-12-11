import java.time.LocalDate;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class Agendamento {
    private LocalDate data;
    private String status;
    private List<Produto> produtos;
    private Notificacao notificacao;

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
    public Agendamento(LocalDate data, List<Produto> produtos){
        statusEmAnalise();
        setData(data);
        setProdutos(produtos);
    }


    //TODOs: Testar mais esse metodo (Muito importante)
    /*
        
        Alteração: Metodo de solicitaçãoDeProfissional pronto.
        Um detalhe, a quantidade pode ser um pouco dificil de implementar dependendo de como vai ser os nossos produtos
        Porque a classe produto em si não tem um atributo quantidade, então teria que sair varrendo o array de produtos
        e vendo quais sao iguais e ai multiplica pelo tempo de produção individual,
        mas pode ser que nao tenha um unico profissional com tempo
        pra fazer a quantidade e sim varios que somando o tempo conseguiriam realizar aquela quantidade de produtos,
        dai que vem a dificuldade, por que teria que fazer mais de um pedido do mesmo produto para profissionais diferentes.
        OBS: Nao sei se ficou claro a explicação, mas qualquer coisa eu explico na aula.
    */
    public void solicitacaoDeProfissional(Profissional[] profissionais){
        Map<Produto, Profissional> alocacaoSimulada = new HashMap<>();
        Map<Profissional, Integer> horasRestantes = new HashMap<>();

        for (Profissional p : profissionais) {
            horasRestantes.put(p, p.HorasDisponiveis());
        }

        for (Produto prod : produtos) {
            boolean encontrado = false;

            for (Profissional p : profissionais) {
                if (p.getEspecialidade() == prod.getEspecialidadeNecessaria()) {
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
            for (Produto prod : produtos) {
                Profissional p = alocacaoSimulada.get(prod);
                System.out.println("Profissional Solicitado:");
                p.imprimeInformacoesProfissional();

                Pedido pedido = new Pedido(
                    this.data,
                    prod.getTempoDePreparo(),
                    prod.getValor(),
                    "Em preparo",
                    1
                );

                boolean ok = p.confirmarPedidoProfissional(pedido);

                if (!ok) {
                    this.status = "Rejeitado";
                    return;
                }
            }

        this.statusAceito();
        
    }
}