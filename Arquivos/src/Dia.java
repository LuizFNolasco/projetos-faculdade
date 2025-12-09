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

    public Dia(String nome, int quantidadeDeHoras, LocalDate data){ 
        setNome(nome);
        setQuantidadeDeHoras(quantidadeDeHoras);
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
    TODOs: Tenho duas sugestões
    
    1 - Implementa so um dos dois métodos e faz a logica do conferirHorario 
        no metodo confirmarHorario, e se tiver horario retorna true indicando que confirmou
        se não retorna false, que ja vai indicar que nao tem horario disponivel
    
    2 - Deixa um metodo so pra confirmar se existe um horarioDisponivel (ConferirHorario) e usa
    ele dentro de confirmarHorario que, esse sim, vai diminuir a quantidade de horas disponiveis
    */
    public Dia conferirHorarioDisponivel(int horasServico){
        if(getQuantidadeDeHoras() >= horasServico){
            diminuiHorasDisponiveis(horasServico);
            return Dia.this;
        }
        return null;
    }

    public void confirmarHorarioDisponivel(int horasServico){
        diminuiHorasDisponiveis(horasServico);
    }
            
}
