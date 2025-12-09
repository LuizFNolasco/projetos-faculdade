import java.time.LocalDate;

public class Candidato extends Pessoa {
	//Alteração: Adicionei o atributo especialidade
	private int tempoExperiencia;
	private String especialidade;
	private float valorDesejado;
	private int horasDisponiveisPorDia;

	
	public int getTempoExperiencia() {
		return tempoExperiencia;
	}

	public void setTempoExperiencia(int tempoExperiencia) {
		this.tempoExperiencia = tempoExperiencia;
	}

	public String getEspecialidade() {
		return especialidade;
	}

	public void setEspecialidade(String especialidade) {
		this.especialidade = especialidade;
	}

	public float getValorDesejado() {
		return valorDesejado;
	}

	public void setValorDesejado(float valorDesejado) {
		this.valorDesejado = valorDesejado;
	}

	public int getHorasDisponiveisPorDia() {
		return horasDisponiveisPorDia;
	}

	public void setHorasDisponiveisPorDia(int horasDisponiveisPorDia) {
		this.horasDisponiveisPorDia = horasDisponiveisPorDia;
	}

	//dataNascimento tava tipada errada
	public Candidato(String cpf, String nome, String email, String telefone, LocalDate dataNascimento,
		int tempoExperiencia, float valorDesejado, int horasDisponiveisPorDia, String especialidade) {
		super(cpf, nome, email, telefone, dataNascimento);
		this.especialidade = especialidade;
		this.tempoExperiencia = tempoExperiencia;
		this.valorDesejado = valorDesejado;
		this.horasDisponiveisPorDia = horasDisponiveisPorDia;
	}

}