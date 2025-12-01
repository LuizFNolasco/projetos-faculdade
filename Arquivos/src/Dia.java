public class Dia {
    private String nome;
    private int quantidadeDeHoras;

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

    public Dia(String nome, int quantidadeDeHoras) {
        setNome(nome);
        setQuantidadeDeHoras(quantidadeDeHoras);
    }

    public void diminuiHorasDisponiveis(int horasGastas){
        quantidadeDeHoras -= horasGastas;
    }
    
    public void imprimeInformacoesDia() {
        System.out.println("Dia: "+getNome());
        System.out.println("Quantidade de horas disponiveis: "+getQuantidadeDeHoras());
    }
}
