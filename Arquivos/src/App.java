import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.ResolverStyle;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class App {
        public static void main(String[] args) throws Exception {
                DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/uuuu")
                                .withResolverStyle(ResolverStyle.STRICT);
                ;

                // Produtos disponíveis
                List<Produto> catalogo = new ArrayList<>();
                catalogo.add(new Produto("Pao", 1, 4, 20, "Padeiro"));
                catalogo.add(new Produto("Doce", 2, 3, 50, "Doceiro"));
                catalogo.add(new Produto("Bolo", 3, 5, 100, "Confeiteiro"));

                // Listas do sistema
                List<Cliente> clientes = new ArrayList<>();

                // clientes pré-cadastrados para testes
                Cliente clienteTeste1 = new Cliente("11122233344", "Maria Silva", "maria@gmail.com", "988881111",
                                LocalDate.parse("10/02/1990", dataFormatada));
                Cliente clienteTeste2 = new Cliente("55566677788", "Carlos Souza", "carlos@gmail.com", "977772222",
                                LocalDate.parse("03/11/1982", dataFormatada));
                clientes.add(clienteTeste1);
                clientes.add(clienteTeste2);

                List<Profissional> profissionais = new ArrayList<>();
                List<Candidato> candidatos = new ArrayList<>();

                // Administrador fixo
                Administrador admin = new Administrador("00000000000", "Admin", "admin@hotmail.com", "000000000",
                                LocalDate.parse("01/01/1980", dataFormatada));

                // Profissionais pre-cadastrados para testes
                Profissional doceiro = new Profissional("22233344455", "Ana Silva", "ana@gmail.com", "900000000",
                                LocalDate.parse("15/03/1992", dataFormatada), "Doceiro", 1, 40.0, 8);
                Profissional confeiteiro = new Profissional("88899900011", "Pedro Silva", "pedro@gmail.com",
                                "955554444", LocalDate.parse("01/09/1988", dataFormatada), "Confeiteiro", 2, 60.0, 8);
                Profissional padeiro = new Profissional("12345678900", "Joao Silva", "joaosilva@gmail.com", "999999999",
                                LocalDate.parse("01/09/1988", dataFormatada), "Padeiro", 3, 10, 8);

                profissionais.add(doceiro);
                profissionais.add(confeiteiro);
                profissionais.add(padeiro);

                Scanner sc = new Scanner(System.in);
                boolean running = true;

                while (running) {
                        Thread.sleep(2000);
                        System.out.println("\n");
                        System.out.println("========================================");
                        System.out.println("|       CONFEITARIA DOCE ENCANTO       |");
                        System.out.println("========================================");
                        System.out.println("|  [1] Adicionar Cliente               |");
                        System.out.println("|  [2] Adicionar Pedido                |         (  )   (  ) ");
                        System.out.println("|  [3] Cancelar Pedido                 |          ) (    )  (");
                        System.out.println("|--------------------------------------|        ..............");
                        System.out.println("|  [4] Adicionar Candidato             |        |            |__");
                        System.out.println("|  [5] Avaliar Candidatos              |        |            |  )");
                        System.out.println("|  [6] Listar Funcionários             |        \\            /_/");
                        System.out.println("|--------------------------------------|         `----------'  ");
                        System.out.println("|  [7] Listar Clientes                 |       _________________");
                        System.out.println("|  [8] Listar Devedores                |");
                        System.out.println("|--------------------------------------|");
                        System.out.println("|  [0] SAIR                            |");
                        System.out.println("========================================");
                        System.out.print(">> Escolha uma opção: ");
                        String esc = sc.nextLine().trim();

                        switch (esc) {
                                case "1": {
                                        String cpf;
                                        while (true) {
                                                try {
                                                        System.out.print("CPF: ");
                                                        cpf = sc.nextLine();
                                                        Pessoa.validarCpf(cpf);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro no CPF: " + e.getMessage());
                                                }
                                        }

                                        String nome;
                                        while (true) {
                                                try {
                                                        System.out.print("Nome: ");
                                                        nome = sc.nextLine();
                                                        Pessoa.validarNome(nome);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        String email;
                                        while (true) {
                                                try {
                                                        System.out.print("Email: ");
                                                        email = sc.nextLine();
                                                        Pessoa.validarEmail(email);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        String tel;
                                        while (true) {
                                                try {
                                                        System.out.print("Telefone: ");
                                                        tel = sc.nextLine();
                                                        Pessoa.validarTelefone(tel);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        LocalDate dataNasc = null;
                                        while (true) {
                                                try {
                                                        System.out.print("Data Nascimento (dd/MM/yyyy): ");
                                                        String dn = sc.nextLine();
                                                        dataNasc = LocalDate.parse(dn, dataFormatada);
                                                        if (dataNasc.isAfter(LocalDate.now())) {
                                                                throw new Exception(
                                                                                "A data não pode ser superior ao dia de hoje");
                                                        }
                                                        break;
                                                } catch (Exception e) {
                                                        System.out.println(
                                                                        "Formato de data invalido, favor digite um formato valido");
                                                }
                                        }

                                        Cliente c = new Cliente(cpf, nome, email, tel, dataNasc);
                                        clientes.add(c);
                                        System.out.println("Cliente adicionado.");
                                        System.out.println();
                                        break;
                                }
                                case "2": {
                                        if (clientes.isEmpty()) {
                                                System.out.println(
                                                                "Nenhum cliente cadastrado. Cadastre um cliente antes.");
                                                System.out.println();
                                                break;
                                        }
                                        System.out.println("Clientes:");
                                        for (int i = 0; i < clientes.size(); i++) {
                                                System.out.println(i + " - " + clientes.get(i).getNome() + " ("
                                                                + clientes.get(i).getCpf() + ")");
                                        }
                                        System.out.println();
                                        System.out.print("Escolha o indice do cliente: ");
                                        int idx = -1;
                                        while (true) {
                                                String s = sc.nextLine().trim();
                                                if (!s.matches("\\d+")) {
                                                        System.out.println(
                                                                        "Indice invalido. Digite um numero inteiro:");
                                                        continue;
                                                }
                                                idx = Integer.parseInt(s);
                                                if (idx < 0 || idx >= clientes.size()) {
                                                        System.out.println("Indice invalido. Digite um indice valido:");
                                                        continue;
                                                }
                                                break;
                                        }
                                        Cliente clienteSelecionado = clientes.get(idx);

                                        List<Produto> sel = new ArrayList<>();
                                        boolean adicionando = true;
                                        while (adicionando) {
                                                System.out.println("Catalogo de produtos:");
                                                for (int i = 0; i < catalogo.size(); i++) {
                                                        Produto p = catalogo.get(i);
                                                        p.imprimeProduto(i);
                                                }
                                                System.out.println();
                                                System.out.print(
                                                                "Escolha indice do produto para adicionar (ou 'f' para finalizar): ");
                                                String line = sc.nextLine().trim();
                                                if (line.equalsIgnoreCase("f")) {
                                                        adicionando = false;
                                                        continue;
                                                }
                                                if (!line.matches("\\d+")) {
                                                        System.out.println("Indice invalido. Digite um numero ou 'f'.");
                                                        System.out.println();
                                                        continue;
                                                }
                                                int pi = Integer.parseInt(line);
                                                if (pi >= 0 && pi < catalogo.size()) {
                                                        sel.add(catalogo.get(pi));
                                                        System.out.println("Produto adicionado.");
                                                        System.out.println();
                                                        clienteSelecionado.setDivida(catalogo.get(pi).getValor());
                                                } else {
                                                        System.out.println("Indice invalido.");
                                                        System.out.println();
                                                }
                                        }

                                        if (sel.isEmpty()) {
                                                System.out.println("Nenhum produto selecionado. Abortando pedido.");
                                                System.out.println();
                                                break;
                                        }

                                        boolean valido = false;
                                        LocalDate dataAgendamento = null;
                                        while (valido == false) {
                                                try {
                                                        System.out.print("Data agendamento (dd/MM/yyyy): ");
                                                        String dataAg = sc.nextLine();
                                                        dataAgendamento = LocalDate.parse(dataAg, dataFormatada);

                                                        if (dataAgendamento.isBefore(LocalDate.now())) {
                                                                throw new Exception(
                                                                                "A data não pode ser anterior ao dia de hoje");
                                                        }
                                                        valido = true;
                                                } catch (Exception e) {
                                                        System.out.println(
                                                                        "Formato de data invalido, favor digite um formato valido");
                                                }
                                        }
                                        Agendamento ag = new Agendamento(dataAgendamento, sel);

                                        clienteSelecionado.setAgendamentos(ag);

                                        ag.solicitacaoDeProfissional(profissionais.toArray(new Profissional[0]),
                                                        clienteSelecionado);
                                        System.out.println("Status do agendamento: " + ag.getStatus());
                                        System.out.println();
                                        break;
                                }
                                case "4": {
                                        String cpfC;
                                        while (true) {
                                                try {
                                                        System.out.print("CPF: ");
                                                        cpfC = sc.nextLine();
                                                        Pessoa.validarCpf(cpfC);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro no CPF: " + e.getMessage());
                                                }
                                        }

                                        String nomeC;
                                        while (true) {
                                                try {
                                                        System.out.print("Nome: ");
                                                        nomeC = sc.nextLine();
                                                        Pessoa.validarNome(nomeC);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        String emailC;
                                        while (true) {
                                                try {
                                                        System.out.print("Email: ");
                                                        emailC = sc.nextLine();
                                                        Pessoa.validarEmail(emailC);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        String telC;
                                        while (true) {
                                                try {
                                                        System.out.print("Telefone: ");
                                                        telC = sc.nextLine();
                                                        Pessoa.validarTelefone(telC);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("Erro: " + e.getMessage());
                                                }
                                        }

                                        LocalDate dataNascC = null;
                                        while (true) {
                                                try {
                                                        System.out.print("Data Nascimento (dd/MM/yyyy): ");
                                                        String dn = sc.nextLine();
                                                        dataNascC = LocalDate.parse(dn, dataFormatada);
                                                        if (dataNascC.isAfter(LocalDate.now())) {
                                                                throw new Exception(
                                                                                "A data não pode ser superior ao dia de hoje");
                                                        }
                                                        break;
                                                } catch (Exception e) {
                                                        System.out.println("Formato invalido");
                                                }
                                        }

                                        int te;
                                        while (true) {
                                                try {
                                                        System.out.print("Tempo experiencia (anos): ");
                                                        String teStr = sc.nextLine();
                                                        if (!teStr.matches("\\d+")) {
                                                                throw new IllegalArgumentException("formato invalido");
                                                        }
                                                        te = Integer.parseInt(teStr);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("formato invalido");
                                                }
                                        }

                                        float valor;
                                        while (true) {
                                                try {
                                                        System.out.print("Valor desejado: ");
                                                        String valorStr = sc.nextLine();
                                                        valor = Float.parseFloat(valorStr);
                                                        if (valor < 0) {
                                                                throw new IllegalArgumentException("formato invalido");
                                                        }
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("formato invalido");
                                                }
                                        }

                                        int hpd;
                                        while (true) {
                                                try {
                                                        System.out.print("Horas disponiveis por dia: ");
                                                        String hpdStr = sc.nextLine();
                                                        if (!hpdStr.matches("\\d+")) {
                                                                throw new IllegalArgumentException("formato invalido");
                                                        }
                                                        hpd = Integer.parseInt(hpdStr);
                                                        break;
                                                } catch (IllegalArgumentException e) {
                                                        System.out.println("formato invalido");
                                                }
                                        }

                                        String esp;
                                        while (true) {
                                                System.out.print("Especialidade: ");
                                                esp = sc.nextLine();
                                                if (esp.length() < 1) {
                                                        System.out.println(
                                                                        "Favor não deixe o campo vazio, digite novamente");
                                                } else {
                                                        break;
                                                }
                                        }

                                        Candidato cand = new Candidato(cpfC, nomeC, emailC, telC, dataNascC, te, valor,
                                                        hpd, esp);
                                        candidatos.add(cand);
                                        System.out.println("Candidato adicionado.");
                                        System.out.println();
                                        break;
                                }
                                case "5": {
                                        if (candidatos.isEmpty()) {
                                                System.out.println("Nenhum candidato para avaliar.");
                                                System.out.println();
                                                break;
                                        }
                                        List<Candidato> aprovados = new ArrayList<>();
                                        for (Candidato cand : candidatos) {
                                                Profissional aprov = admin.aprovaCandidato(cand);
                                                if (aprov != null) {
                                                        profissionais.add(aprov);
                                                        aprovados.add(cand);
                                                        System.out.println("Candidato aprovado: " + cand.getNome());
                                                } else {
                                                        System.out.println("Candidato rejeitado: " + cand.getNome());
                                                }
                                        }
                                        // remove aprovados da lista de candidatos
                                        candidatos.removeAll(aprovados);
                                        System.out.println();
                                        break;
                                }
                                case "7": {
                                        if (clientes.isEmpty()) {
                                                System.out.println("Nenhum cliente cadastrado.");
                                                System.out.println();
                                                break;
                                        }
                                        System.out.println("Lista de clientes:\n");
                                        for (Cliente c : clientes) {
                                                c.imprimeInformacoesPessoa();
                                                System.out.println();
                                        }
                                        break;
                                }
                                case "6": {
                                        if (profissionais.isEmpty()) {
                                                System.out.println("Estamos sem profissionais no momento.");
                                                System.out.println();
                                                break;
                                        }
                                        System.out.println("Lista de profissionais:\n");
                                        for (Profissional p : profissionais) {
                                                p.imprimeInformacoesProfissional();
                                                System.out.println();
                                        }
                                        break;
                                }
                                case "3": {
                                        if (clientes.isEmpty()) {
                                                System.out.println("Nenhum cliente cadastrado.");
                                                System.out.println();
                                                break;
                                        }
                                        System.out.println("Clientes:");
                                        for (int i = 0; i < clientes.size(); i++) {
                                                System.out.println(i + " - " + clientes.get(i).getNome() + " ("
                                                                + clientes.get(i).getCpf() + ")");
                                        }
                                        System.out.println();
                                        System.out.print("Escolha o indice do cliente dono do pedido: ");
                                        int idxCliente = -1;
                                        while (true) {
                                                String s = sc.nextLine().trim();
                                                if (!s.matches("\\d+")) {
                                                        System.out.println(
                                                                        "Indice invalido. Digite um numero inteiro:");
                                                        continue;
                                                }
                                                idxCliente = Integer.parseInt(s);
                                                if (idxCliente < 0 || idxCliente >= clientes.size()) {
                                                        System.out.println("Indice invalido. Digite um indice valido:");
                                                        continue;
                                                }
                                                break;
                                        }
                                        Cliente clienteSelecionadoParaCancelamento = clientes.get(idxCliente);

                                        // coletar pedidos do cliente entre todos os profissionais
                                        List<Profissional> profsParaPedidos = new ArrayList<>();
                                        List<Pedido> pedidosDoCliente = new ArrayList<>();

                                        for (int i = 0; i < profissionais.size(); i++) {
                                                Profissional prof = profissionais.get(i);
                                                // primeiro, checar se há um pedido em preparo que pertence ao cliente
                                                Pedido atual = prof.getPedidoAtual();
                                                if (atual != null && clienteSelecionadoParaCancelamento.getCpf()
                                                                .equals(atual.getClienteCpf())) {
                                                        pedidosDoCliente.add(atual);
                                                        profsParaPedidos.add(prof);
                                                }

                                                List<Pedido> lista = prof.getPedidos();
                                                if (lista == null)
                                                        continue;
                                                for (int j = 0; j < lista.size(); j++) {
                                                        Pedido ped = lista.get(j);
                                                        if (ped != null && clienteSelecionadoParaCancelamento.getCpf()
                                                                        .equals(ped.getClienteCpf())) {
                                                                pedidosDoCliente.add(ped);
                                                                profsParaPedidos.add(prof);
                                                        }
                                                }
                                        }

                                        if (pedidosDoCliente.isEmpty()) {
                                                System.out.println("Nenhum pedido encontrado para este cliente.");
                                                System.out.println();
                                                break;
                                        }

                                        System.out.println("Pedidos do cliente:");
                                        for (int i = 0; i < pedidosDoCliente.size(); i++) {
                                                Pedido ped = pedidosDoCliente.get(i);
                                                Profissional prof = profsParaPedidos.get(i);
                                                String nomeProduto = (ped.getProduto() != null
                                                                ? ped.getProduto().getNome()
                                                                : "N/A");
                                                System.out.println(i + " - ID: " + ped.getId() + " | Produto: "
                                                                + nomeProduto + " | Status: " + ped.getStatus()
                                                                + " | Profissional: " + prof.getNome());
                                        }
                                        System.out.println();
                                        System.out.print("Escolha o indice do pedido para cancelar: ");
                                        int idxPedido = -1;
                                        while (true) {
                                                String s = sc.nextLine().trim();
                                                if (!s.matches("\\d+")) {
                                                        System.out.println(
                                                                        "Indice de pedido invalido. Digite um numero inteiro:");
                                                        continue;
                                                }
                                                idxPedido = Integer.parseInt(s);
                                                if (idxPedido < 0 || idxPedido >= pedidosDoCliente.size()) {
                                                        System.out.println(
                                                                        "Indice de pedido invalido. Digite um indice valido:");
                                                        continue;
                                                }
                                                break;
                                        }
                                        Pedido pedidoParaCancelar = pedidosDoCliente.get(idxPedido);
                                        Profissional profResponsavel = profsParaPedidos.get(idxPedido);
                                        int codigo = profResponsavel.cancelarPedidoPorId(
                                                        pedidoParaCancelar.getId(),
                                                        clienteSelecionadoParaCancelamento.getCpf());

                                        if (codigo == 0) {
                                                System.out.println("Falha ao cancelar o pedido.");
                                                System.out.println();
                                                break;
                                        }
                                        // ajustar dívida do cliente conforme tipo de cancelamento
                                        double valorPedido = pedidoParaCancelar.getValor();
                                        if (codigo == 1) {
                                                // cancelado durante preparo -> cobrar 50% (cliente é cobrado metade =>
                                                // reduzir dívida pela metade do valor)
                                                clienteSelecionadoParaCancelamento.setDivida(-(valorPedido / 2.0));
                                        } else if (codigo == 2) {
                                                // cancelado na fila -> devolver integral
                                                clienteSelecionadoParaCancelamento.setDivida(-valorPedido);
                                        }
                                        System.out.println();
                                        break;
                                }
                                case "8":
                                        int i = 0;
                                        for (Cliente c : clientes) {
                                                if (c.getDivida() > 0.1) {
                                                        System.out.println("O cliente " + c.getNome()
                                                                        + " deve um total de: R$" + c.getDivida());
                                                        i++;
                                                }
                                        }
                                        if (i == 0) {
                                                System.out.println("\nNão há devedores");
                                        }
                                        break;
                                case "0": {
                                        running = false;
                                        break;
                                }
                                default:
                                        System.out.println("Opcao invalida.");
                        }
                }

                sc.close();
                System.out.println("Encerrando sistema.");
                System.out.println();
        }
}
