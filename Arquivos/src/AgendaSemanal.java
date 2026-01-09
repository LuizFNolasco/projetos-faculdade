import java.time.LocalDate;

public class AgendaSemanal {
    private int qtdDiasDisponiveis;
    private Dia[] dias;

    public void setQtdDiasDisponiveis(int qtdDiasDisponiveis) {
        this.qtdDiasDisponiveis = qtdDiasDisponiveis;
    }
    
    public void setDias(int qtdDiasDisponiveis) {
        this.dias = new Dia[qtdDiasDisponiveis];
    }

    public void inicializarDias() {
        String[] nomes = { "Segunda", "Terca", "Quarta", "Quinta", "Sexta" };

        LocalDate dataInicial = LocalDate.of(2025, 12, 1);

        for (int i = 0; i < qtdDiasDisponiveis; i++) {
            LocalDate data = dataInicial.plusDays(i); // cada dia soma 1
            dias[i] = new Dia(nomes[i], data);
        }
    }

    public AgendaSemanal() {
        setQtdDiasDisponiveis(5);
        setDias(qtdDiasDisponiveis);
        inicializarDias();
    }

    public void imprimeInformacoesAgenda() {
        System.out.println("Quantidade de horas disponiveis na semana: " + HorasDisponiveisAgenda());
    }

    public int HorasDisponiveisAgenda() {
        int totalHoras = 0;
        for (int i = 0; i < this.dias.length; i++) {
            totalHoras += dias[i].getQuantidadeDeHoras();
        }

        return totalHoras;
    }

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
