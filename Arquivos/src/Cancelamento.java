public class Cancelamento {

    /*
    TODOs: É bom o cancelamento ter o cliente que esta fazendo o cancelamento
    Já que provavelmente o cancelamento vai precisar dos dados do cliente pra penalizar ele
    e dependendo de como vai ser o calculo de penalidade, vai precisar dos dados do pedido tambem.
    */

    private String descricao;
    /*
    TODOs: Acho que o tempo minimo de cancelamento nao precisa ser atributo, é so definir na implementação mesmo
    Ou
    Da pra definir ele como atributo static tambem e colocar um valor padrao pra todo cancelamento
    */
    private int tempoMinimoDeCancelamento; // Acho que nao vai precisar

    // Acho melhor a penalidade ser float ou double.
    private float penalidade;

    Cancelamento(String descricao, int tempoMinimoDeCancelamento, int penalidade) {
        setDescricao(descricao);
        setTempoMinimoDeCancelamento(tempoMinimoDeCancelamento);
        setPenalidade(penalidade);
    }


    public String getDescricao() {
        return descricao;
    }
    public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

    public int getTempoMinimoDeCancelamento() {
        return tempoMinimoDeCancelamento;
    }
    public void setTempoMinimoDeCancelamento(int tempoMinimoDeCancelamento) {
        this.tempoMinimoDeCancelamento = tempoMinimoDeCancelamento;
    }

    public float getPenalidade() {
        return penalidade;
    }
    public void setPenalidade(float penalidade) {
        this.penalidade = penalidade;
    }

    /*
    TODOs: Preencher calculaPenalidade()

    public float calculaPenalidade(float penalidade) {

    }
    */

    



}
