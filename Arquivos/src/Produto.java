
abstract public class Produto {
    private String nome;
    private int id;
    private int tempoDePreparo;

    Produto(String nome, int id, int tempoDePreparo) {
        setNome(nome);
        setId(id);
        setTempoDePreparo(tempoDePreparo);
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
