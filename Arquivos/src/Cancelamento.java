public class Cancelamento {
    private Cliente cliente;
    private double penalidade;
    private Agendamento agend;

    public double getPenalidade() {
        return penalidade;
    }
    public void setPenalidade(double penalidade) {
        this.penalidade = penalidade;
    }

    public void ativarDivida(Cliente cliente){
        this.cliente = cliente;

        cliente.setDivida(getPenalidade());
    }

    public Cliente getCliente() {
        return cliente;
    }

    public void setCliente(Cliente cliente) {
        this.cliente = cliente;
    }

    public Agendamento getAgend() {
        return agend;
    }
    public void setAgend(Agendamento agend) {
        this.agend = agend;
        agend.statusCancelado();
    }

    Cancelamento(double penalidade, Cliente cliente) {
        setPenalidade(penalidade);
        setCliente(cliente);
        ativarDivida(cliente);
    }
}
