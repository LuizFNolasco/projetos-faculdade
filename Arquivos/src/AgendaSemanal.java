import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class AgendaSemanal{
    // Alteração: Retirei a qtdHorasDiarias -> É atributo do dia
    private int qtdDiasDisponiveis;
    private Dia[] dias;

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
            dias[i] = new Dia(nomes[i], data);
        }
    }

    public AgendaSemanal(){
        setQtdDiasDisponiveis(5);
        setDias(qtdDiasDisponiveis);
        inicializarDias();
    }

    public void imprimeInformacoesAgenda() {
        System.out.println("Quantidade de dias disponiveis: " + getQtdDiasDisponiveis());
        for (Dia dia : dias) {
            dia.imprimeInformacoesDia();
        }
    }

    public Dia conferirHorarioProfissional(int horasServico){
        for (Dia dia : dias){
            if(dia.conferirHorarioDisponivel(horasServico) != null) {
                return dia;
            }
        }
        return null;
    }

    public void marcarHorarioProfissional(int horasServico){
        Dia diaDisponivel = conferirHorarioProfissional(horasServico);
        if(diaDisponivel != null) {
            diaDisponivel.confirmarHorarioDisponivel(horasServico);
        }
    }

}
