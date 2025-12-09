
abstract public class Produto {
    //Adicionei valor ao produto

    private float valor;
    private String nome;
    private int id;
    private int tempoDePreparo;

    Produto(String nome, int id, int tempoDePreparo, float valor) {
        setValor(valor);
        setNome(nome);
        setId(id);
        setTempoDePreparo(tempoDePreparo);
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

    

}
