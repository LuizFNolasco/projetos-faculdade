import os
import re
import statistics

# --- 1. CONFIGURAÇÃO ---
base = "benchmarks/isic19/results"           # Pasta pai que contém as pastas folder_1, folder_2...
prefixo = "vgg-13_metablock_folder"         # Prefixo comum das pastas
subpasta_alvo = "best_metrics"               # Subpasta onde está o metrics.txt
arquivo = "metrics.txt"
variaveis = ["Accuracy", "Balanced accuracy", "AUC"]
pasta_destino = "/home/luiznolasco/resultados/isic/metablock"
arquivo_saida = "resultado_vgg.txt"

nomes_curtos = {
    "Accuracy": "ACC",
    "Balanced accuracy": "BACC",
    "AUC": "AUC"
}

# --- 2. COLETA DOS DADOS ---
dados = {var: [] for var in variaveis}

# Lista e ordena apenas as pastas com o prefixo correto
pastas_fold = sorted([
    p for p in os.listdir(base)
    if p.startswith(prefixo) and os.path.isdir(os.path.join(base, p))
])

print(f"{len(pastas_fold)} pastas encontradas:")
for p in pastas_fold:
    print(f"  {p}")

for pasta in pastas_fold:
    caminho_metrics = os.path.join(base, pasta, subpasta_alvo, arquivo)

    if not os.path.exists(caminho_metrics):
        print(f"  ⚠ Arquivo não encontrado: {caminho_metrics}")
        continue

    print(f"  ✔ Lendo: {caminho_metrics}")

    with open(caminho_metrics, 'r', encoding='utf-8') as f:
        conteudo = f.read()

    for var in variaveis:
        busca = re.findall(rf"{var}\s*[:=]\s*([0-9.]+)", conteudo)
        if busca:
            dados[var].append(float(busca[-1]))

# --- 3. GRAVAÇÃO ---
os.makedirs(pasta_destino, exist_ok=True)
caminho_final = os.path.join(pasta_destino, arquivo_saida)

with open(caminho_final, 'w', encoding='utf-8') as f:

    # Bloco 1: Valores por fold
    for var in variaveis:
        valores = dados[var]
        if valores:
            sigla = nomes_curtos.get(var, var.upper())
            linha_valores = " + ".join(f"{v:.4f}" for v in valores)
            f.write(f"{sigla} = {linha_valores}\n")

    f.write("\n")

    # Bloco 2: Médias e Desvios
    for var in variaveis:
        valores = dados[var]
        if valores:
            sigla = nomes_curtos.get(var, var.upper())
            media = statistics.mean(valores)
            desvio = statistics.stdev(valores) if len(valores) > 1 else 0.0
            f.write(f"Media {sigla} = {media:.4f}\n")
            f.write(f"Desvio padrao {sigla} = {desvio:.4f}\n\n")

print(f"\nArquivo salvo em: {caminho_final}")
print(f"Folds lidos: { {k: len(v) for k, v in dados.items()} }")