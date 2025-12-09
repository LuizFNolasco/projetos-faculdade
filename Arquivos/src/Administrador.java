import java.time.LocalDate;

public class Administrador extends Pessoa {

	/* public Administrador() {
		super();
	}*/

	public Administrador(String cpf, String nome, String email, String telefone, LocalDate dataNascimento) {
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
		if (c == null) return null;

		//Alterei pro profissional precisar de 8 horas diarias ao invés de 6
		boolean criterios = c.getTempoExperiencia() > 5 && c.getValorDesejado() < 3000f
				&& c.getHorasDisponiveisPorDia() >= 8;

		if (!criterios)
			return null;

		//Alteração: Criei a instâcia do profissional que vai ser retornado
		//TODOs:Como vai ser definido o id?
		Profissional aprovado = new Profissional(c.getCpf(), c.getNome(), c.getEmail(), c.getTelefone(), c.getDataNascimento(), c.getEspecialidade(), 0, c.getValorDesejado(), c.getTempoExperiencia());
		return aprovado;
	}

}