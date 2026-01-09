import java.time.LocalDate;

public abstract class Pessoa {
    private String cpf;
    private String nome;
    private String email;
    private String telefone;
    private LocalDate dataNascimento;

    public String getCpf() {
        return cpf;
    }

    public void setCpf(String cpf) {
        // valida CPF: exatamente 11 dígitos numéricos
        if (cpf == null || !cpf.matches("\\d{11}")) {
            throw new IllegalArgumentException("CPF inválido: deve conter exatamente 11 dígitos numéricos.");
        }
        this.cpf = cpf;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        // valida nome não vazio
        if (nome == null || nome.trim().isEmpty()) {
            throw new IllegalArgumentException("Nome inválido: não pode ser vazio.");
        }
        this.nome = nome;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        // valida email simples (permitir apenas domínios comuns usados no app)
        if (email == null || !(email.contains("@gmail.com") || email.contains("@hotmail.com")
                || email.contains("@outlook.com"))) {
            throw new IllegalArgumentException("Email inválido: use @gmail.com, @hotmail.com ou @outlook.com.");
        }
        this.email = email;
    }

    public String getTelefone() {
        return telefone;
    }

    public void setTelefone(String telefone) {
        // valida telefone: exatamente 9 dígitos numéricos
        if (telefone == null || !telefone.matches("\\d{9}")) {
            throw new IllegalArgumentException("Telefone inválido: deve conter exatamente 9 dígitos numéricos.");
        }
        this.telefone = telefone;
    }

    public LocalDate getDataNascimento() {
        return dataNascimento;
    }

    public void setDataNascimento(LocalDate dataNascimento) {
        this.dataNascimento = dataNascimento;
    }

    public Pessoa(String cpf, String nome, String email, String telefone, LocalDate dataNascimento) {
        setCpf(cpf);
        setNome(nome);
        setEmail(email);
        setTelefone(telefone);
        setDataNascimento(dataNascimento);
    }

    void imprimeInformacoesPessoa() {
        System.out.println("Nome: " + getNome());
        System.out.println("CPF: " + getCpf());
        System.out.println("Email: " + getEmail());
        System.out.println("Telefone: " + getTelefone());
        System.out.println("Data de Nascimento: " + getDataNascimento());
    }

    public static void validarCpf(String cpf) {
        if (cpf == null || !cpf.matches("\\d{11}")) {
            throw new IllegalArgumentException("CPF inválido: deve conter exatamente 11 dígitos numéricos.");
        }
    }

    public static void validarNome(String nome) {
        if (nome == null || nome.trim().isEmpty()) {
            throw new IllegalArgumentException("Nome inválido: não pode ser vazio.");
        }
    }

    public static void validarEmail(String email) {
        if (email == null || !(email.contains("@gmail.com") || email.contains("@hotmail.com")
                || email.contains("@outlook.com"))) {
            throw new IllegalArgumentException("Email inválido: use @gmail.com, @hotmail.com ou @outlook.com.");
        }
    }

    public static void validarTelefone(String telefone) {
        if (telefone == null || !telefone.matches("\\d{9}")) {
            throw new IllegalArgumentException("Telefone inválido: deve conter exatamente 9 dígitos numéricos.");
        }
    }

}
