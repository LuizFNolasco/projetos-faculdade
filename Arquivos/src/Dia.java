import java.time.LocalDate;

public class Dia {
    private String nome;
    private int quantidadeDeHoras;
    private final int maxHoras;
    private LocalDate data;

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getNome() {
        return nome;
    }

    public void setQuantidadeDeHoras(int quantidadeDeHoras) {
        this.quantidadeDeHoras = quantidadeDeHoras;
    }

    public int getQuantidadeDeHoras() {
        return quantidadeDeHoras;
    }

    public void setData(LocalDate data) {
        this.data = data;
    }

    public LocalDate getData() {
        return data;
    }

    public Dia(String nome, LocalDate data) {
        // maxHoras fixo em 8
        this.maxHoras = 8;
        setQuantidadeDeHoras(8);
        setNome(nome);
        setData(data);
    }

    public void diminuiHorasDisponiveis(int horasGastas) {
        quantidadeDeHoras -= horasGastas;
        if (quantidadeDeHoras < 0) {
            quantidadeDeHoras = 0;
        }
    }

    public void aumentaHorasDisponiveis(int horas) {
        quantidadeDeHoras += horas;
        if (quantidadeDeHoras > maxHoras) {
            quantidadeDeHoras = maxHoras;
        }
    }

}
