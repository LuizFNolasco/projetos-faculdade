import java.time.LocalDate;
import java.time.format.DateTimeFormatter;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Hello, World!");
        //TODOs: Usar isso aqui para formatar a variavel data para o padrão brasileiro
        DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/yyyy");

        LocalDate dataNascimento = LocalDate.parse("20/05/1985", dataFormatada);
        
        // ============================
        // INSTÂNCIAS PARA TESTE
        // ============================

        // Produtos
        Produto pao = new Produto("Pão",1, 4, 20, "Padeiro");
        Produto doce = new Produto("Doce", 2, 3, 50, "Doceiro");
        Produto bolo = new Produto("Bolo",3,  5, 100, "Confeiteiro");

        // Clientes
        Cliente cliente1 = new Cliente("111.222.333-44", "Maria Silva", "maria@gmail.com", "98888-1111", LocalDate.parse("10/02/1990", dataFormatada));
        Cliente cliente2 = new Cliente("555.666.777-88", "Carlos Souza", "carlos@gmail.com", "97777-2222", LocalDate.parse("03/11/1982", dataFormatada));

        // Profissionais adicionais
        Profissional doceiro = new Profissional("222.333.444-55", "Ana Doceira", "ana@gmail.com", "90000-0000", LocalDate.parse("15/03/1992", dataFormatada), "Doceiro", 1, 40.0, 8);
        Profissional confeiteiro = new Profissional("888.999.000-11", "Pedro Confeiteiro", "pedro@gmail.com", "95555-4444", LocalDate.parse("01/09/1988", dataFormatada), "Confeiteiro", 1, 60.0, 6);
        Profissional padeiro = new Profissional("123.456.789-00", "Joao Silva", "joaosilva@gmail.com", "99999-9999", LocalDate.parse("01/09/1988", dataFormatada), "Padeiro", 1, 10, 5);
        

        // Simulação de reservas dos profissionais
        doceiro.confirmarDiaProfissional(8);
        confeiteiro.confirmarDiaProfissional(4);

        // Lista de produtos para um agendamento
        java.util.List<Produto> listaProdutos1 = new java.util.ArrayList<>();
        listaProdutos1.add(pao);
        listaProdutos1.add(bolo);
        listaProdutos1.add(doce);

        // Criação de um agendamento
        Agendamento agendamento1 = new Agendamento( LocalDate.parse("12/12/2025", dataFormatada), listaProdutos1);

        // Solicitação dos profissionais
        Profissional[] profissionais = { padeiro, doceiro, confeiteiro };
        agendamento1.solicitacaoDeProfissional(profissionais);

        // Resultado final
        System.out.println("Status do agendamento: " + agendamento1.getStatus());
        System.out.println("Produtos restantes (se recusado): " + agendamento1.getProdutos());
    }
}
