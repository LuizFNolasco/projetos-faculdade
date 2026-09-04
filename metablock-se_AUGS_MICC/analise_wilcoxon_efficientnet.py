import os
import sys
import re
import glob
import numpy as np
from collections import defaultdict

# Adiciona a pasta pai (IC_Final) ao path para poder importar utils.stats
sys.path.append(os.path.abspath(".."))
from utils.stats import statistical_test

# --- 1. DADOS DO EFFICIENTNET (EXTRAÍDOS DOS NOTEBOOKS) ---
EFFICIENTNET_RESULTS = {
    "augsburg_multi": {
        "Loss": [1.6418, 1.5535, 1.5921, 1.7754, 1.4879], 
        "Accuracy": [0.4000, 0.3529, 0.4314, 0.4118, 0.5200], 
        "Balanced accuracy": [0.1667, 0.1786, 0.1568, 0.1364, 0.2186]
    },
    "augsburg_binario": {
        "Loss": [0.5840, 0.5490, 0.6626, 0.5907, 0.5506], 
        "Accuracy": [0.5882, 0.6078, 0.5600, 0.6600, 0.6275], 
        "Balanced accuracy": [0.5227, 0.5076, 0.5217, 0.4853, 0.5690]
    },
    "miccai": {
        "Loss": [0.6674, 0.6823, 0.6745, 0.5158, 0.6739], 
        "Accuracy": [0.6842, 0.2778, 0.6667, 0.7619, 0.5714], 
        "Balanced accuracy": [0.5714, 0.5000, 0.6818, 0.8214, 0.6538]
    }
}

# --- 2. CONFIGURAÇÃO DA LEITURA DOS BENCHMARKS ---
# Altere aqui para qual dataset deseja analisar (augsburg_multi, augsburg_binario, ou miccai)
DATASET_ALVO = "miccai"

# Caminhos base para cada dataset
PATHS = {
    "augsburg_multi": "benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss",
    "augsburg_binario": "benchmarks/augsburg/results_Binario/opt_adam_early_stop_loss", # Ajuste se for outro nome
    "miccai": "benchmarks_miccai/miccai/results/opt_adam_early_stop_loss" # Ajuste o subcaminho exato se precisar
}

base_dir = PATHS[DATASET_ALVO]
arquivo = "metrics.txt"
variaveis = ["Loss", "Accuracy", "Balanced accuracy"]

def nested_dict():
    return defaultdict(lambda: {var: [] for var in variaveis})
dados = defaultdict(nested_dict)

print(f"--- INICIANDO ANÁLISE PARA: {DATASET_ALVO.upper()} ---")
padrao_busca = os.path.join(base_dir, "*", "metablock-se", "*", "missing_0", "folder_*", "best_metrics", arquivo)
arquivos_encontrados = glob.glob(padrao_busca)

if not arquivos_encontrados:
    print(f"ATENÇÃO: Nenhum arquivo encontrado no caminho: {padrao_busca}")
    print("Verifique se o caminho no dicionário PATHS está correto para este dataset.")
else:
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

    # --- 3. EXECUTANDO O TESTE ESTATÍSTICO E SALVANDO EM ARQUIVO ---
    arquivo_resultado_txt = f"resultados_estatisticos_{DATASET_ALVO}.txt"
    caminho_txt = os.path.join(base_dir, arquivo_resultado_txt)
    
    # Redireciona a saída do terminal (print) para o arquivo de texto
    original_stdout = sys.stdout
    with open(caminho_txt, "w", encoding="utf-8") as f_out:
        sys.stdout = f_out
        
        for metrica in ["Accuracy", "Balanced accuracy"]:
            print(f"\n==========================================================")
            print(f" TESTE ESTATÍSTICO PARA A MÉTRICA: {metrica.upper()}")
            print(f"==========================================================")

            for vlm, llms in dados.items():
                print(f"\n---> VLM: {vlm}")
                dados_matriz = []
                nomes_algoritmos = []
                
                # Adiciona o Baseline (EfficientNet) primeiro
                dados_matriz.append(EFFICIENTNET_RESULTS[DATASET_ALVO][metrica])
                nomes_algoritmos.append("EfficientNet (Baseline)")

                # Adiciona os resultados dos LLMs
                for llm, valores in llms.items():
                    if len(valores[metrica]) > 0:
                        dados_matriz.append(valores[metrica])
                        nomes_algoritmos.append(f"{vlm}+{llm}")

                # Transforma em array NumPy e executa o teste
                matriz_np = np.array(dados_matriz)
                
                # A função do seu professor já dá os prints automáticos
                resultado = statistical_test(data=matriz_np, alg_names=nomes_algoritmos)

    # Restaura o print para o terminal
    sys.stdout = original_stdout
    print(f"\nOs resultados dos testes estatísticos foram salvos com sucesso no arquivo:")
    print(f"{caminho_txt}")
