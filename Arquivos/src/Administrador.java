public class Administrador extends Pessoa {

	public Administrador() {
		super();
	}

	public Administrador(String cpf, String nome, String email, String telefone, java.util.Date dataNascimento) {
		super(cpf, nome, email, telefone, dataNascimento);
	}

	// gerenciaAgendamentos()
	public void gerenciaAgendamentos() {
		// TODO: implementar lógica de gerenciamento de agendamentos
	}

	// gerenciaPoliticas()
	public void gerenciaPoliticas() {
		// TODO: implementar lógica de políticas (regras do sistema)
	}

	public Profissional aprovaCandidato(Candidato c) {
		if (c == null)
			return null;

		boolean criterios = c.getTempoExperiencia() > 5 && c.getValorDesejado() < 3000f
				&& c.getHorasDisponiveisPorDia() > 6;

		if (!criterios)
			return null;

		return new Profissional();
	}

}