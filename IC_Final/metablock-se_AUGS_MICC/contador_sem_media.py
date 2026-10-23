import os
import re
import glob
import json
from collections import defaultdict

# --- 1. CONFIGURAÇÃO ---
base = "/home/luiz_felipe/IC_Final/metablock-se_AUGS_MICC/benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss"
arquivo = "metrics.txt"
variaveis = ["Loss", "Accuracy", "Balanced accuracy"]
pasta_destino = "/home/luiz_felipe/IC_Final/metablock-se_AUGS_MICC/"
arquivo_saida = "resultados_folds_multiclass.json"

# --- 2. COLETA DOS DADOS ---
def nested_dict():
    return defaultdict(lambda: {var: [] for var in variaveis})

dados = defaultdict(nested_dict)

padrao_busca = os.path.join(base, "*", "metablock-se", "*", "missing_0", "folder_*", "best_metrics", arquivo)
arquivos_encontrados = glob.glob(padrao_busca)

print(f"Foram encontrados {len(arquivos_encontrados)} arquivos metrics.txt.")

for caminho in arquivos_encontrados:
    partes = caminho.split(os.sep)
    
    llm = partes[-5] 
    vlm = partes[-7] 
    
    with open(caminho, 'r', encoding='utf-8') as f:
        conteudo = f.read()

    for var in variaveis:
        busca = re.findall(rf"{var}\s*[:=]\s*([0-9.]+)", conteudo)
        if busca:
            dados[vlm][llm][var].append(float(busca[-1]))

# --- 3. GRAVAÇÃO ---
os.makedirs(pasta_destino, exist_ok=True)
caminho_final = os.path.join(pasta_destino, arquivo_saida)

# Converte o defaultdict para um dicionário padrão do Python para poder salvar em JSON
dados_json = {vlm: {llm: dict(metricas) for llm, metricas in llms.items()} for vlm, llms in dados.items()}

with open(caminho_final, 'w', encoding='utf-8') as f:
    json.dump(dados_json, f, indent=4)

print(f"\nDados completos (separados por fold) extraídos com sucesso!")
print(f"Salvo em: {caminho_final}")
