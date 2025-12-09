public class Candidato extends Pessoa {
	private int tempoExperiencia;
	private float valorDesejado;
	private int horasDisponiveisPorDia;

	
	public int getTempoExperiencia() {
		return tempoExperiencia;
	}

	public void setTempoExperiencia(int tempoExperiencia) {
		this.tempoExperiencia = tempoExperiencia;
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

	public Candidato(String cpf, String nome, String email, String telefone, java.util.Date dataNascimento,
			int tempoExperiencia, float valorDesejado, int horasDisponiveisPorDia) {
		super(cpf, nome, email, telefone, dataNascimento);
		this.tempoExperiencia = tempoExperiencia;
		this.valorDesejado = valorDesejado;
		this.horasDisponiveisPorDia = horasDisponiveisPorDia;
	}

}