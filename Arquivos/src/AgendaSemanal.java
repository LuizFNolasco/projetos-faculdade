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

    public void inicializarDias(String[] nomes) {
        if (nomes.length != qtdDiasDisponiveis) {
            throw new IllegalArgumentException("Quantidade de nomes diferente da quantidade de dias.");
        }

        for (int i = 0; i < qtdDiasDisponiveis; i++) {
            dias[i] = new Dia(nomes[i], qtdHorasDiarias);
        }
    }

    public AgendaSemanal(int qtdHorasDiarias, int qtdDiasDisponiveis, String[] nomes){
        setQtdHorasDiarias(qtdHorasDiarias);
        setQtdDiasDisponiveis(qtdDiasDisponiveis);
        setDias(qtdDiasDisponiveis);
        inicializarDias(nomes);
    }

    public void imprimeInformacoesAgenda() {
        System.out.println("Quantidade de horas diarias: " + getQtdHorasDiarias());
        System.out.println("Quantidade de dias disponiveis: " + getQtdDiasDisponiveis());
        for (Dia dia : dias) {
            dia.imprimeInformacoesDia();
        }
    }

}
