import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class AgendaSemanal{
    private int qtdHorasDiarias;
    private int qtdDiasDisponiveis;
    private Dia[] dias;

    public void setQtdHorasDiarias(int qtdHorasDiarias){
        this.qtdHorasDiarias = qtdHorasDiarias;
    }

    public int getQtdHorasDiarias(){
        return qtdHorasDiarias;
    }   

    public void setQtdDiasDisponiveis(int qtdDiasDisponiveis){
        this.qtdDiasDisponiveis = qtdDiasDisponiveis;
    }

    public int getQtdDiasDisponiveis(){
        return qtdDiasDisponiveis;
    }

    public void setDias(int qtdDiasDisponiveis){
        this.dias = new Dia[qtdDiasDisponiveis];
    }

    public void inicializarDias() {
        DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/yyyy");
        String[] nomes = {"Segunda", "Terca", "Quarta", "Quinta", "Sexta"};

        LocalDate dataInicial = LocalDate.of(2025, 12, 1);

        for (int i = 0; i < qtdDiasDisponiveis; i++) {
            LocalDate data = dataInicial.plusDays(i); // cada dia soma 1
            System.out.println(data.format(dataFormatada));
            dias[i] = new Dia(nomes[i], qtdHorasDiarias, data);
        }
    }

    public AgendaSemanal(int qtdHorasDiarias){
        setQtdHorasDiarias(qtdHorasDiarias);
        setQtdDiasDisponiveis(5);
        setDias(qtdDiasDisponiveis);
        inicializarDias();
    }

    public void imprimeInformacoesAgenda() {
        System.out.println("Quantidade de horas diarias: " + getQtdHorasDiarias());
        System.out.println("Quantidade de dias disponiveis: " + getQtdDiasDisponiveis());
        for (Dia dia : dias) {
            dia.imprimeInformacoesDia();
        }
    }

    public Dia conferirHorarioProfissional(int horasServico){
        for (Dia dia : dias){
            Dia diaSemana = dia.conferirHorarioDisponivel(horasServico);
            if(diaSemana != null) {
                return diaSemana;
            }
        }
        return null;
    }

    public void marcarHorarioProfissional(Dia dia, int horasServico){
        dia.conferirHorarioDisponivel(horasServico);
    }

}
