import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Hello, World!");
        //TODOS: Usar isso aqui para formatar a variavel data para o padrão brasileiro
        DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/yyyy");

        LocalDate dataNascimento = LocalDate.parse("20/05/1985", dataFormatada);

        String[] dias = new String[]{"Segunda", "Terca", "Quarta", "Quinta", "Sexta"};
        Profissional padeiro = new Profissional("123.456.789-00", "Joao Silva", "joaosilva@gmail.com", "99999-9999", dataNascimento, "Padeiro", 1, 50.0, 5, 8, 5, dias);
        padeiro.imprimeInformacoesProfissional();
    }
}
