import os
import re
import statistics
import glob
from collections import defaultdict

# --- 1. CONFIGURAÇÃO ---
base = "/home/luiz_felipe/IC_Final/metablock-se_AUGS_MICC/benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss"
arquivo = "metrics.txt"
# MUDANÇA 1: Trocamos a AUC pela Loss na nossa lista de busca
variaveis = ["Loss", "Accuracy", "Balanced accuracy"]
pasta_destino = "/home/luiz_felipe/IC_Final/metablock-se_AUGS_MICC/"
arquivo_saida = "resultado_augsburg_multiclass.txt"

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

with open(caminho_final, 'w', encoding='utf-8') as f:
    for vlm, llms in dados.items():
        # Escreve o nome do VLM
        f.write(f"{vlm}:\n\n")
        
        for llm, metricas in llms.items():

            print(f"[{vlm} - {llm}] Valores encontrados nos folds:")
            print(f"Loss: {metricas['Loss']} | Acc: {metricas['Accuracy']} | Bacc: {metricas['Balanced accuracy']}\n")
            # Calcula a média de todos os folds encontrados para as três métricas
            media_loss = statistics.mean(metricas["Loss"]) if metricas["Loss"] else 0.0
            media_acc = statistics.mean(metricas["Accuracy"]) if metricas["Accuracy"] else 0.0
            media_bacc = statistics.mean(metricas["Balanced accuracy"]) if metricas["Balanced accuracy"] else 0.0
            
            # MUDANÇA 2: Escreve exatamente na estrutura "Modelo: Loss | Acc | Bacc"
            f.write(f"{llm}: {media_loss:.4f} | {media_acc:.4f} | {media_bacc:.4f}\n")
        
        # Pula uma linha no final de cada VLM para deixar organizado
        f.write("\n")

print(f"\nResumo criado com sucesso! Salvo em: {caminho_final}")