import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public final class Cliente extends Pessoa {
    private List<Agendamento> agendamentos = new ArrayList<>();
    private double divida = 0;

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
