import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

public class Profissional extends Pessoa {
    // Alteração: Adicionei uma lista de pedidos pendentes é inicializada como null

    private List<Pedido> pedidos;
    private String especialidade;
    private int id;
    private double valorMinimoPedido;
    private int tempoExperiencia; // em anos
    private AgendaSemanal agenda;

    public List<Pedido> getPedidos() {
        return pedidos;
    }

    public void setPedidos(List<Pedido> pedidos) {
        this.pedidos = pedidos;
    }

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

    public void setAgenda(){
        this.agenda = new AgendaSemanal();
    }
    
    public Profissional(String cpf, String nome, String email, String telefone, LocalDate dataNascimento, String especialidade, int id, double valorMinimoPedido, int tempoExperiencia){
        super(cpf, nome, email, telefone, dataNascimento);
        setEspecialidade(especialidade);
        setId(id);
        setValorMinimoPedido(valorMinimoPedido);
        setTempoExperiencia(tempoExperiencia);
        setPedidos(new ArrayList<>());
        setAgenda();
    }

    public void imprimeInformacoesProfissional(){
        super.imprimeInformacoesPessoa();
        System.out.println("Especialidade: "+getEspecialidade());
        System.out.println("ID: "+getId());
        System.out.println("Valor Minimo do Pedido: "+getValorMinimoPedido());  
        agenda.imprimeInformacoesAgenda();
    }

    //Alteração: Metodo novo que insere um pedido novo na lista de pedidos ao profissional e utiliza
    // o metodo confirmarDiaProfissional para descontar as horas semanais.
    public boolean confirmarPedidoProfissional(Pedido pedido) {
        if(confirmarDiaProfissional(pedido.getDuracao())) {
            this.pedidos.add(pedido);
            return true;
        }
        return false;
    }

    public int HorasDisponiveis() {
       return this.agenda.HorasDisponiveisAgenda();
    }

    public boolean conferirAgendaProfissional(int horasServico){
        if(agenda.conferirHorarioProfissional(horasServico)) {
            return true;
        }
        return false;
    }

    //Alteração: Deixei esse metodo privado por que acho que ele so deveria ser utilizado no metodo confirmarPedido
    /* Inclusive, acho que vale a pena deixar private todos os metodos que so vamos utilizar na mesma classe
       Até pra deixar menos poluido e a gente nao acabar confundindo na hora de usar outras classes.
    */
    private boolean confirmarDiaProfissional(int horasServico){
        if(agenda.marcarHorarioProfissional(horasServico)) {
            return true;
        }
        return false;

    }


}
