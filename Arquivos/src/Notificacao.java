import java.util.Scanner;

public class Notificacao {
    public String nomeDestinatario;
    public String emailDestinatario;

    public void setNomeDestinatario(String nomeDestinatario){
        this.nomeDestinatario = nomeDestinatario;
    }

    public String getNomeDestinatario(){
        return nomeDestinatario;
    }

    public void setEmailDestinatario(String emailDestinatario){
        this.emailDestinatario = emailDestinatario;
    }

    public String getEmailDestinatario(){
        return emailDestinatario;
    }

    public Notificacao(String nomeDestinatario, String emailDestinatario){
        setNomeDestinatario(nomeDestinatario);
        setEmailDestinatario(emailDestinatario);
    }

    public void notificacaoPadrao(){
        System.out.println("Mensagem enviada para o email: " + getEmailDestinatario());
        System.out.println("Caro(a) senhor(a) "+getNomeDestinatario());
        System.out.println("Esta e uma notificacao da (nome empresa) para tratar de:");
    }

    public int notificacaoEspecifica(String assunto){
        System.out.printf("%s\n", assunto);
        Scanner entrada = new Scanner(System.in);
        System.out.println("Digite a opcao desejada: ");
        int opcao = entrada.nextInt();
        entrada.close();
        return opcao;
    }

}
