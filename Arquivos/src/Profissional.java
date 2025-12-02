import java.time.LocalDate;

public class Profissional extends Pessoa {
    private String especialidade;
    private int id;
    private double valorMinimoPedido;
    private int tempoExperiencia; // em anos
    private AgendaSemanal agenda;

    public void setEspecialidade(String especialidade){
        this.especialidade = especialidade;
    }

    public String getEspecialidade(){
        return especialidade;
    }

    public void setId(int id){
        this.id = id;
    }

    public int getId(){
        return id;
    }

    public void setValorMinimoPedido(double valorMinimoPedido){
        this.valorMinimoPedido = valorMinimoPedido;
    }

    public double getValorMinimoPedido(){
        return valorMinimoPedido;
    }

    public void setTempoExperiencia(int tempoExperiencia){
        this.tempoExperiencia = tempoExperiencia;
    }

    public int getTempoExperiencia(){
        return tempoExperiencia;
    }

    public void setAgenda(int qtdHorasDiarias){
        this.agenda = new AgendaSemanal(qtdHorasDiarias);
    }
    
    public Profissional(String cpf, String nome, String email, String telefone, LocalDate dataNascimento, String especialidade, int id, double valorMinimoPedido, int tempoExperiencia, int qtdHorasDiarias){
        super(cpf, nome, email, telefone, dataNascimento);
        setEspecialidade(especialidade);
        setId(id);
        setValorMinimoPedido(valorMinimoPedido);
        setTempoExperiencia(tempoExperiencia);
        setAgenda(qtdHorasDiarias);
    }

    public void imprimeInformacoesProfissional(){
        super.imprimeInformacoesPessoa();
        System.out.println("Especialidade: "+getEspecialidade());
        System.out.println("ID: "+getId());
        System.out.println("Valor Minimo do Pedido: "+getValorMinimoPedido());  
        agenda.imprimeInformacoesAgenda();
    }

    public Dia conferirAgendaProfissional(int horasServico){
        return agenda.conferirHorarioProfissional(horasServico);
    }

    public void confirmarDiaProfissional(Dia dia, int horasServico){
        agenda.marcarHorarioProfissional(dia, horasServico);
    }


}
