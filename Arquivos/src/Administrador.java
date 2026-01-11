import java.time.LocalDate;
import java.time.Period;

public class Administrador extends Pessoa {

	public Administrador(String cpf, String nome, String email, String telefone, LocalDate dataNascimento) {
		super(cpf, nome, email, telefone, dataNascimento);
	}

	public Profissional aprovaCandidato(Candidato c) {
		if (c == null) return null;

		boolean criterios = c.getTempoExperiencia() > 5 && c.getValorDesejado() < 3000f
				&& c.getHorasDisponiveisPorDia() >= 8 && Period.between(c.getDataNascimento(), LocalDate.now()).getYears() >= 18;

		if (!criterios)
			return null;
		
		Profissional aprovado = new Profissional(c.getCpf(), c.getNome(), c.getEmail(), c.getTelefone(), c.getDataNascimento(), c.getEspecialidade(), c.getValorDesejado(), c.getTempoExperiencia());
		return aprovado;
	}

}