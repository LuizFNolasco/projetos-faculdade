public class Produto {
    private String especialidadeNecessaria;
    private float valor;
    private String nome;
    private int id;
    private int tempoDePreparo;

    public String getEspecialidadeNecessaria() {
        return especialidadeNecessaria;
    }

    public void setEspecialidadeNecessaria(String especialidadeNecessaria) {
        this.especialidadeNecessaria = especialidadeNecessaria;
    }

    public float getValor() {
        return valor;
    }

    public void setValor(float valor) {
        this.valor = valor;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getTempoDePreparo() {
        return tempoDePreparo;
    }

    public void setTempoDePreparo(int tempoDePreparo) {
        this.tempoDePreparo = tempoDePreparo;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public void imprimeProduto(int indice){
        System.out.println(indice + " - " +getNome() + " ("+getEspecialidadeNecessaria() + ") - tempo: "+getTempoDePreparo() + "h - R$" +getValor());
    }

    Produto(String nome, int id, int tempoDePreparo, float valor, String especialidadeNecessaria) {
        setValor(valor);
        setNome(nome);
        setId(id);
        setTempoDePreparo(tempoDePreparo);
        setEspecialidadeNecessaria(especialidadeNecessaria);
    }


}
