import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.ResolverStyle;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class App {
        public static void main(String[] args) throws Exception {
                DateTimeFormatter dataFormatada = DateTimeFormatter.ofPattern("dd/MM/uuuu").withResolverStyle(ResolverStyle.STRICT);;

                // Produtos disponíveis
                List<Produto> catalogo = new ArrayList<>();
                catalogo.add(new Produto("Pao", 1, 4, 20, "Padeiro"));
                catalogo.add(new Produto("Doce", 2, 3, 50, "Doceiro"));
                catalogo.add(new Produto("Bolo", 3, 5, 100, "Confeiteiro"));

                // Listas do sistema
                List<Cliente> clientes = new ArrayList<>();

                // clientes pré-cadastrados para testes
                Cliente clienteTeste1 = new Cliente("11122233344", "Maria Silva", "maria@gmail.com", "988881111", LocalDate.parse("10/02/1990", dataFormatada));
                Cliente clienteTeste2 = new Cliente("55566677788", "Carlos Souza", "carlos@gmail.com", "977772222", LocalDate.parse("03/11/1982", dataFormatada));
                clientes.add(clienteTeste1);
                clientes.add(clienteTeste2);

                List<Profissional> profissionais = new ArrayList<>();
                List<Candidato> candidatos = new ArrayList<>();

                // Administrador fixo
                Administrador admin = new Administrador("00000000000", "Admin", "admin@hotmail.com", "000000000", LocalDate.parse("01/01/1980", dataFormatada));

                // Profissionais pre-cadastrados para testes
                Profissional doceiro = new Profissional("22233344455", "Ana Silva", "ana@gmail.com", "900000000", LocalDate.parse("15/03/1992", dataFormatada), "Doceiro", 1, 40.0, 8);
                Profissional confeiteiro = new Profissional("88899900011", "Pedro Silva", "pedro@gmail.com", "955554444", LocalDate.parse("01/09/1988", dataFormatada), "Confeiteiro", 2, 60.0, 8);
                Profissional padeiro = new Profissional("12345678900", "Joao Silva", "joaosilva@gmail.com", "999999999", LocalDate.parse("01/09/1988", dataFormatada), "Padeiro", 3, 10, 8);

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
                        System.out.println("|  [7] Cancelar Pedido                 |          ) (    )  (");
                        System.out.println("|--------------------------------------|        ..............");
                        System.out.println("|  [3] Adicionar Candidato             |        |            |__");
                        System.out.println("|  [4] Avaliar Candidatos              |        |            |  )");
                        System.out.println("|  [6] Listar Funcionários             |        \\            /_/");
                        System.out.println("|--------------------------------------|         `----------'  ");
                        System.out.println("|  [5] Listar Clientes                 |       _________________");
                        System.out.println("|  [8] Listar Devedores                |");
                        System.out.println("|--------------------------------------|");
                        System.out.println("|  [0] SAIR                            |");
                        System.out.println("========================================");
                        System.out.print(">> Escolha uma opção: ");
                        String esc = sc.nextLine().trim();

                        switch (esc) {
                                case "1": {
                                        System.out.print("CPF: ");
                                        String cpf = sc.nextLine();
                                        while(cpf.length() != 11){
                                                System.out.println("O cpf não possui 11 digitos, favor digite novamente");
                                                cpf = sc.nextLine();
                                        }

                                        System.out.print("Nome: ");
                                        String nome = sc.nextLine();
                                        while(nome.length() < 1){
                                                System.out.println("O nome não pode ser vazio, favor digite novamente");
                                                nome = sc.nextLine();
                                        }
                                        
                                        System.out.print("Email: ");
                                        String email = sc.nextLine();
                                        while(!email.contains("@gmail.com") && !email.contains("@hotmail.com") && !email.contains("@outlook.com")){
                                                System.out.println("Formato de email incorreto");
                                                email = sc.nextLine();
                                        }

                                        System.out.print("Telefone: ");
                                        String tel = sc.nextLine();
                                        while(tel.length() != 9){
                                                System.out.println("O numero de telefone nao possui 9 digitos, favor digite novamente");
                                                tel = sc.nextLine();
                                        }
                                        
                                        boolean valido = false;
                                        LocalDate dataNasc = null;
                                        while(valido == false){
                                                try{
                                                        System.out.print("Data Nascimento (dd/MM/yyyy): ");
                                                        String dn = sc.nextLine();
                                                        dataNasc = LocalDate.parse(dn, dataFormatada);
                                                        
                                                        if(dataNasc.isAfter(LocalDate.now())){
                                                                throw new Exception("A data não pode ser superior ao dia de hoje");
                                                        }
                                                        valido = true;
                                                }
                                                catch(Exception e){
                                                        System.out.println("Formato de data invalido, favor digite um formato valido");
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
                                                System.out.println("Nenhum cliente cadastrado. Cadastre um cliente antes.");
                                                System.out.println();
                                                break;
                                        }
                                        System.out.println("Clientes:");
                                        for (int i = 0; i < clientes.size(); i++) {
                                                System.out.println(i + " - " + clientes.get(i).getNome() + " ("+ clientes.get(i).getCpf() + ")");
                                        }
                                        System.out.println();
                                        System.out.print("Escolha o indice do cliente: ");
                                        int idx = Integer.parseInt(sc.nextLine());
                                        if (idx < 0 || idx >= clientes.size()) {
                                                System.out.println("Indice invalido.");
                                                System.out.println();
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
                                                System.out.print("Escolha indice do produto para adicionar (ou 'f' para finalizar): ");
                                                String line = sc.nextLine();
                                                if (line.equalsIgnoreCase("f")) {
                                                        adicionando = false;
                                                } 
                                                else {
                                                        int pi = Integer.parseInt(line);
                                                        if (pi >= 0 && pi < catalogo.size()) {
                                                                sel.add(catalogo.get(pi));
                                                                System.out.println("Produto adicionado.");
                                                                System.out.println();

                                                                clienteSelecionado.setDivida(catalogo.get(pi).getValor());
                                                        } 
                                                        else {
                                                                System.out.println("Indice invalido.");
                                                                System.out.println();
                                                        }
                                                }
                                        }

                                        if (sel.isEmpty()) {
                                                System.out.println("Nenhum produto selecionado. Abortando pedido.");
                                                System.out.println();
                                                break;
                                        }

                                        boolean valido = false;
                                        LocalDate dataAgendamento = null;
                                        while(valido == false){
                                                try{
                                                        System.out.print("Data agendamento (dd/MM/yyyy): ");
                                                        String dataAg = sc.nextLine();
                                                        dataAgendamento = LocalDate.parse(dataAg, dataFormatada);
                                                        
                                                        if(dataAgendamento.isBefore(LocalDate.now())){
                                                                throw new Exception("A data não pode ser anterior ao dia de hoje");
                                                        }
                                                        valido = true;
                                                }catch(Exception e){
                                                        System.out.println("Formato de data invalido, favor digite um formato valido");
                                                }
                                        }
                                        Agendamento ag = new Agendamento(dataAgendamento, sel);

                                        clienteSelecionado.setAgendamentos(ag);

                                        ag.solicitacaoDeProfissional(profissionais.toArray(new Profissional[0]), clienteSelecionado);
                                        System.out.println("Status do agendamento: " + ag.getStatus());
                                        System.out.println();
                                        break;
                                }
                                case "3": {
                                        System.out.print("CPF: ");
                                        String cpf = sc.nextLine();
                                        while(cpf.length() != 11){
                                                System.out.println("O cpf não possui 11 digitos, favor digite novamente");
                                                cpf = sc.nextLine();
                                        }

                                        System.out.print("Nome: ");
                                        String nome = sc.nextLine();
                                        while(nome.length() < 1){
                                                System.out.println("O nome não pode ser vazio, favor digite novamente");
                                                nome = sc.nextLine();
                                        }

                                        System.out.print("Email: ");
                                        String email = sc.nextLine();
                                        while(!email.contains("@gmail.com") && !email.contains("@hotmail.com") && !email.contains("@outlook.com")){
                                                System.out.println("Formato de email incorreto");
                                                email = sc.nextLine();
                                        }

                                        System.out.print("Telefone: ");
                                        String tel = sc.nextLine();
                                        while(tel.length() != 9){
                                                System.out.println("O numero de telefone nao possui 9 digitos, favor digite novamente");
                                                tel = sc.nextLine();
                                        }

                                        boolean valido = false;
                                        LocalDate dataNasc = null;
                                        while(valido == false){
                                                try{
                                                        System.out.print("Data Nascimento (dd/MM/yyyy): ");
                                                        String dn = sc.nextLine();
                                                        dataNasc = LocalDate.parse(dn, dataFormatada);
                                                        
                                                        if(dataNasc.isAfter(LocalDate.now())){
                                                                throw new Exception("A data não pode ser superior ao dia de hoje");
                                                        }
                                                        valido = true;
                                                }catch(Exception e){
                                                        System.out.println("Formato de data invalido, favor digite um formato valido");
                                                }
                                        }

                                        System.out.print("Tempo experiencia (anos): ");
                                        int te = Integer.parseInt(sc.nextLine());
                                        while(te < 0){
                                                System.out.println("Tempo de experiencia invalido, favor digite novamente");
                                                te = Integer.parseInt(sc.nextLine());
                                        }

                                        System.out.print("Valor desejado: ");
                                        float valor = Float.parseFloat(sc.nextLine());
                                        while(valor < 0){
                                                System.out.println("Valor invalido, favor digite novamente");
                                                valor = Float.parseFloat(sc.nextLine());
                                        }

                                        System.out.print("Horas disponiveis por dia: ");
                                        int hpd = Integer.parseInt(sc.nextLine());
                                        while(hpd < 0){
                                                System.out.println("Número de horas invalido, favor digite novamente");
                                                hpd = Integer.parseInt(sc.nextLine());
                                        }

                                        System.out.print("Especialidade: ");
                                        String esp = sc.nextLine();
                                        while(esp.length() < 1){
                                                System.out.println("Favor não deixe o campo vazio, digite novamente");
                                                esp = sc.nextLine();
                                        }
                                        
                                        Candidato cand = new Candidato(cpf, nome, email, tel, dataNasc, te, valor, hpd, esp);
                                        candidatos.add(cand);
                                        System.out.println("Candidato adicionado.");
                                        System.out.println();
                                        break;
                                }
                                case "4": {
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
                                                } 
                                                else {
                                                        System.out.println("Candidato rejeitado: " + cand.getNome());
                                                }
                                        }
                                        // remove aprovados da lista de candidatos
                                        candidatos.removeAll(aprovados);
                                        System.out.println();
                                        break;
                                }
                                case "5": {
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
                                case "7": {
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
                                        int idxCliente = Integer.parseInt(sc.nextLine());
                                        if (idxCliente < 0 || idxCliente >= clientes.size()) {
                                                System.out.println("Indice invalido.");
                                                System.out.println();
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
                                                        if (ped != null && clienteSelecionadoParaCancelamento.getCpf().equals(ped.getClienteCpf())) {
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
                                                String nomeProduto = (ped.getProduto() != null ? ped.getProduto().getNome() : "N/A");
                                                System.out.println(i + " - ID: " + ped.getId() + " | Produto: " + nomeProduto + " | Status: " + ped.getStatus() + " | Profissional: " + prof.getNome());
                                        }
                                        System.out.println();
                                        System.out.print("Escolha o indice do pedido para cancelar: ");
                                        int idxPedido = Integer.parseInt(sc.nextLine());
                                        if (idxPedido < 0 || idxPedido >= pedidosDoCliente.size()) {
                                                System.out.println("Indice de pedido invalido.");
                                                System.out.println();
                                                break;
                                        }
                                        Pedido pedidoParaCancelar = pedidosDoCliente.get(idxPedido);

                                        Cancelamento cancel = new Cancelamento(pedidoParaCancelar.getValor()/2, clienteSelecionadoParaCancelamento);

                                        clienteSelecionadoParaCancelamento.setDivida(-pedidoParaCancelar.getValor());

                                        Profissional profResponsavel = profsParaPedidos.get(idxPedido);
                                        boolean cancelOk = profResponsavel.cancelarPedidoPorId(
                                                        pedidoParaCancelar.getId(),
                                                        clienteSelecionadoParaCancelamento.getCpf());
                                        if (cancelOk) {
                                                System.out.println("\nPedido cancelado com sucesso no profissional: "
                                                                + profResponsavel.getNome());
                                        } 
                                        else {
                                                System.out.println("Falha ao cancelar o pedido.");
                                        }
                                        System.out.println();
                                        break;
                                }
                                case "8":
                                        int i = 0;
                                        for(Cliente c : clientes){
                                                if(c.getDivida() > 0.1){
                                                        System.out.println("O cliente " + c.getNome() + " deve um total de: R$" + c.getDivida());
                                                        i++;
                                                }
                                        }
                                        if(i == 0){
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
