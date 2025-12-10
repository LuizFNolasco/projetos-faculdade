import java.time.LocalDate;

public class Dia {
    private String nome;
    private int quantidadeDeHoras;
    private LocalDate data;


    public void setNome(String nome){
        this.nome = nome;
    }

    public String getNome(){
        return nome;
    }

    public void setQuantidadeDeHoras(int quantidadeDeHoras){
        this.quantidadeDeHoras = quantidadeDeHoras;
    }

    public int getQuantidadeDeHoras() {
        return quantidadeDeHoras;
    }

    public void setData(LocalDate data){
        this.data = data;
    }

    public LocalDate getData() {
        return data;
    }

    //Alteração: Construtor não recebe mais quantidadeDeHoras -> é fixo em 8
    public Dia(String nome, LocalDate data){ 
        setQuantidadeDeHoras(8);
        setNome(nome);
        setData(data);
    }

    public void diminuiHorasDisponiveis(int horasGastas){
        quantidadeDeHoras -= horasGastas;
    }
    
    public void imprimeInformacoesDia() {
        System.out.println("Dia: "+getNome()+", Data: "+getData());
        System.out.println("Quantidade de horas disponiveis: "+getQuantidadeDeHoras());
    }

    /*
    Alteração: conferirHorarioDisponivel não diminui mais as horas apenas verifica se existe horario livre
            quem diminui as horas é o metodo confirmarHorarioDisponivel
    */
    public Dia conferirHorarioDisponivel(int horasServico) {
        if(getQuantidadeDeHoras() >= horasServico){
            return Dia.this;
        }

        return null;
    }

    public void confirmarHorarioDisponivel(int horasServico){
        if(conferirHorarioDisponivel(horasServico) != null) {
            diminuiHorasDisponiveis(horasServico);
        }
    }
            
}
