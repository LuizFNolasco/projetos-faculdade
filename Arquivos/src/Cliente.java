import java.time.LocalDate;
import java.util.List;

public final class Cliente extends Pessoa {
    //Alteração: Adicionei lista de agendamentos e notificações.
    private List<Agendamento> agendamentos;
    private List<Notificacao> notificacoes;

    public List<Agendamento> getAgendamentos() {
        return agendamentos;
    }

    public void setAgendamentos(List<Agendamento> agendamentos) {
        this.agendamentos = agendamentos;
    }

    public List<Notificacao> getNotificacoes() {
        return notificacoes;
    }

    public void setNotificacoes(List<Notificacao> notificacoes) {
        this.notificacoes = notificacoes;
    }

    public Cliente(String cpf, String nome, String email, String telefone, LocalDate dataNascimento ){
        super(cpf, nome, email, telefone, dataNascimento);
    }
}
