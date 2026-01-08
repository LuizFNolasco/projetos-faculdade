import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public final class Cliente extends Pessoa {
    //Alteração: Adicionei lista de agendamentos e notificações.
    // TODOs: Aqui tem que ter alguma coisa relacionada a dinheiro, pra quando ele tiver que pagar a loja pelo produto ou pela taxa de cancelamento
    private List<Agendamento> agendamentos = new ArrayList<>();
    private double divida = 0;

    public List<Agendamento> getAgendamentos() { // TODOs: não está
        return agendamentos;
    }

    public void setAgendamentos(Agendamento agendamento) {
        this.agendamentos.add(agendamento);
    }

    public void setDivida(double divida){
        this.divida += divida;
    }

   public double getDivida() {
        return divida;
    }

    public Cliente(String cpf, String nome, String email, String telefone, LocalDate dataNascimento ){
        super(cpf, nome, email, telefone, dataNascimento);
    }
}
