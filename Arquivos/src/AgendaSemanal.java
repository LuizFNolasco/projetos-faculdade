import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class AgendaSemanal {
    // Alteração: Retirei a qtdHorasDiarias -> É atributo do dia
    private int qtdDiasDisponiveis;
    private Dia[] dias;

    public void setQtdDiasDisponiveis(int qtdDiasDisponiveis) {
        this.qtdDiasDisponiveis = qtdDiasDisponiveis;
    }

    public int getQtdDiasDisponiveis() {
        return qtdDiasDisponiveis;
    }

    public void setDias(int qtdDiasDisponiveis) {
        this.dias = new Dia[qtdDiasDisponiveis];
    }

    public void inicializarDias() {
        DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/yyyy");
        String[] nomes = { "Segunda", "Terca", "Quarta", "Quinta", "Sexta" };

        LocalDate dataInicial = LocalDate.of(2025, 12, 1);

        for (int i = 0; i < qtdDiasDisponiveis; i++) {
            LocalDate data = dataInicial.plusDays(i); // cada dia soma 1
            // System.out.println(data.format(dataFormatada));
            dias[i] = new Dia(nomes[i], data);
        }
    }

    public AgendaSemanal() {
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

    public int HorasDisponiveisAgenda() {
        int totalHoras = 0;
        for (int i = 0; i < this.dias.length; i++) {
            totalHoras += dias[i].getQuantidadeDeHoras();
        }

        return totalHoras;
    }

    // ALTERAÇÃO GRANDE:
    /*
     * Mudei a logica de alocação de horas dos profissionais.
     * - Agora o conferirHorarioProfissional confere se a semana inteira do
     * profissional tem horas suficientes
     * pro serviço e nao se existe um dia capaz de alocar todo o serviço.
     * 
     * - E pro marcarHorarioProfissional, usa conferirHorarioProfissional pra ver se
     * tem tempo na semana e, caso tenha,
     * vai alocando a maior quantidade de horas pra cada dia.
     * 
     * exemplo: Serviço de 16 horas
     * ANTES DA MARCAÇÃO
     * Agenda -> Segunda Terça Quarta Quinta Sexta
     * horas -> 6 2 8 4 6
     * 
     * DEPOIS
     * Agenda -> Segunda Terça Quarta Quinta Sexta
     * horas -> 0 0 0 4 6
     * 
     */
    /*
     * Inclusive, talvez vale a pena ate deletar a classe dia
     * Do jeito antigo se o profissional tem 10 horas semanais sendo 2 horas em cada
     * dia da semana e ele
     * receber um pedido de 4 horas, esse pedido nao vai ser aprovado.
     * 
     */
    public boolean conferirHorarioProfissional(int horasServico) {
        if (HorasDisponiveisAgenda() >= horasServico)
            return true;
        return false;
    }

    public boolean marcarHorarioProfissional(int horasServico) {
        if (conferirHorarioProfissional(horasServico)) {
            int horasRestantes = horasServico;
            while (horasRestantes > 0) {
                for (Dia dia : this.dias) {
                    int horasDisponiveis = dia.getQuantidadeDeHoras();
                    if (horasDisponiveis > horasRestantes) {
                        dia.diminuiHorasDisponiveis(horasRestantes);
                        return true;
                    } else {
                        horasRestantes -= horasDisponiveis;
                        dia.diminuiHorasDisponiveis(horasDisponiveis);
                    }
                }
            }

        }

        return false;
    }

    // nova função: libera horas de volta na semana (ex: quando um pedido é
    // cancelado)
    public boolean liberarHoras(int horasADevolver) {
        int horasRestantes = horasADevolver;
        for (int i = 0; i < this.dias.length && horasRestantes > 0; i++) {
            Dia dia = this.dias[i];
            int antes = dia.getQuantidadeDeHoras();
            dia.aumentaHorasDisponiveis(horasRestantes);
            int depois = dia.getQuantidadeDeHoras();
            int acrescido = depois - antes;
            horasRestantes -= acrescido;
        }
        return true;
    }

}
