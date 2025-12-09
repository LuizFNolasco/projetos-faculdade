public class Cancelamento {

    /*
    TODOs: Precisamos vincular o cancelamento com o cliente pra penalizar ele, e com o pedido pra liberar horasDisponiveis
    */

    private String descricao;
    /*
    TODOs: Já que o tempoMinimoDeCancelamento vai ser especifico pra cada pedido, acho legal ele ser um date Dia
    */
    private Dia tempoMinimoDeCancelamento; // Acho que nao vai precisar

    // Acho melhor a penalidade ser float ou double.
    private float penalidade;

    Cancelamento(String descricao, Dia tempoMinimoDeCancelamento, int penalidade) {
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

    public Dia getTempoMinimoDeCancelamento() {
        return tempoMinimoDeCancelamento;
    }
    public void setTempoMinimoDeCancelamento(Dia tempoMinimoDeCancelamento) {
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
