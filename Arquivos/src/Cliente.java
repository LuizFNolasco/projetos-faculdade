import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public final class Cliente extends Pessoa {
    //Alteração: Adicionei lista de agendamentos e notificações.
    // TODOs: Aqui tem que ter alguma coisa relacionada a dinheiro, pra quando ele tiver que pagar a loja pelo produto ou pela taxa de cancelamento
    private List<Agendamento> agendamentos = new ArrayList<>();
    private List<Notificacao> notificacoes = new ArrayList<>(); // TODOs: não está

    public List<Agendamento> getAgendamentos() { // TODOs: não está
        return agendamentos;
    }

    public void setAgendamentos(Agendamento agendamento) {
        this.agendamentos.add(agendamento);
    }

    public List<Notificacao> getNotificacoes() { // TODOs: não está
        return notificacoes;
    }

    public void setNotificacoes(List<Notificacao> notificacoes) { // TODOs: não está
        this.notificacoes = notificacoes;
    }

    public Cliente(String cpf, String nome, String email, String telefone, LocalDate dataNascimento ){
        super(cpf, nome, email, telefone, dataNascimento);
    }
}
