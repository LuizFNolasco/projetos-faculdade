import os
import sys
import re
import glob
import numpy as np
from collections import defaultdict

# Adiciona a pasta pai (IC_Final) ao path para poder importar utils.stats
sys.path.append(os.path.abspath(".."))
from utils.stats import statistical_test
from baselines_extraidos import BASELINES

# --- CONFIGURAÇÃO DA LEITURA DOS BENCHMARKS ---
# Altere aqui para qual dataset deseja analisar (augsburg_multi, augsburg_binario, ou miccai)
DATASET_ALVO = "augsburg_binario"

# Caminhos base para cada dataset
PATHS = {
    "augsburg_multi": "benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss",
    "augsburg_binario": "benchmarks/augsburg/results_Binario/opt_adam_early_stop_loss",
    "miccai": "benchmarks_miccai/miccai/results/opt_adam_early_stop_loss"
}

base_dir = PATHS[DATASET_ALVO]
arquivo = "metrics.txt"
variaveis = ["Loss", "Accuracy", "Balanced accuracy"]

def nested_dict():
    return defaultdict(lambda: {var: [] for var in variaveis})
dados = defaultdict(nested_dict)

padrao_busca = os.path.join(base_dir, "*", "metablock-se", "*", "missing_0", "folder_*", "best_metrics", arquivo)
arquivos_encontrados = glob.glob(padrao_busca)

if not arquivos_encontrados:
    print(f"ATENÇÃO: Nenhum arquivo encontrado no caminho: {padrao_busca}")
    sys.exit()

for caminho in arquivos_encontrados:
    partes = caminho.split(os.sep)
    llm = partes[-5] # O backbone (resnet-50, mobilenet, etc)
    vlm = partes[-7] # O vlm
    
    with open(caminho, 'r', encoding='utf-8') as f:
        conteudo = f.read()

    for var in variaveis:
        busca = re.findall(rf"{var}\s*[:=]\s*([0-9.]+)", conteudo)
        if busca:
            dados[vlm][llm][var].append(float(busca[-1]))

# Inverte o dicionário para agrupar por BACKBONE (llm) e depois por VLM
dados_por_backbone = defaultdict(nested_dict)
for vlm, llms in dados.items():
    for llm, metricas in llms.items():
        dados_por_backbone[llm][vlm] = metricas

# --- EXECUTANDO O TESTE ESTATÍSTICO E SALVANDO EM ARQUIVO ---
arquivo_resultado_txt = f"resultados_pareados_{DATASET_ALVO}.txt"
caminho_txt = os.path.join(base_dir, arquivo_resultado_txt)

original_stdout = sys.stdout
with open(caminho_txt, "w", encoding="utf-8") as f_out:
    sys.stdout = f_out
    
    print(f"--- ANÁLISE PAREADA (MODELO vs MODELO) PARA: {DATASET_ALVO.upper()} ---")

    for metrica in ["Accuracy", "Balanced accuracy"]:
        print(f"\n==========================================================")
        print(f" MÉTRICA: {metrica.upper()}")
        print(f"==========================================================")

        for backbone, vlms in dados_por_backbone.items():
            print(f"\n---> BACKBONE: {backbone.upper()}")
            
            # Verifica se temos o baseline desse backbone
            if backbone not in BASELINES:
                print(f"Atenção: Baseline para {backbone} não encontrado nos dados extraídos.")
                continue
                
            dados_matriz = []
            nomes_algoritmos = []
            
            # 1. Adiciona o Baseline correspondente
            baseline_vals = BASELINES[backbone][DATASET_ALVO].get(metrica, [])
            if not baseline_vals:
                print(f"Atenção: Valores de {metrica} para o baseline de {backbone} não encontrados.")
                continue
                
            dados_matriz.append(baseline_vals)
            nomes_algoritmos.append(f"Baseline ({backbone})")

            # 2. Adiciona os resultados do Metablock para esse mesmo backbone
            for vlm, valores in vlms.items():
                if len(valores[metrica]) > 0:
                    dados_matriz.append(valores[metrica])
                    nomes_algoritmos.append(f"Metablock ({vlm}+{backbone})")

            if len(dados_matriz) < 2:
                print("Sem modelos suficientes do Metablock para comparar.")
                continue

            matriz_np = np.array(dados_matriz)
            
            # A função do seu professor já dá os prints automáticos
            resultado = statistical_test(data=matriz_np, alg_names=nomes_algoritmos)

# Restaura o print para o terminal
sys.stdout = original_stdout
print(f"\nFeito! Análise modelo-a-modelo (pareada) concluída com sucesso.")
print(f"Os resultados foram salvos no arquivo:")
print(f"{caminho_txt}")
