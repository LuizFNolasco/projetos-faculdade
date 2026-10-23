import os
import sys
import re
import glob
import numpy as np
import pandas as pd
from collections import defaultdict
from scipy import stats

sys.path.append(os.path.abspath(".."))
from baselines_extraidos import BASELINES

# --- CONFIGURAÇÃO ---
DATASET_ALVO = "augsburg_binario" # Troque por augsburg_binario ou augsburg_multi se quiser
PATHS = {
    "augsburg_multi": "benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss",
    "augsburg_binario": "benchmarks/augsburg/results_Binario/opt_adam_early_stop_loss",
    "miccai": "benchmarks_miccai/miccai/results/opt_adam_early_stop_loss"
}
base_dir = PATHS[DATASET_ALVO]
arquivo = "metrics.txt"

METRICA = "Balanced accuracy"

dados = defaultdict(lambda: defaultdict(list))
padrao_busca = os.path.join(base_dir, "*", "metablock-se", "*", "missing_0", "folder_*", "best_metrics", arquivo)
arquivos_encontrados = glob.glob(padrao_busca)

if not arquivos_encontrados:
    print(f"ATENÇÃO: Nenhum arquivo encontrado no caminho: {padrao_busca}")
    sys.exit()

for caminho in arquivos_encontrados:
    partes = caminho.split(os.sep)
    llm = partes[-5] # Backbone
    vlm = partes[-7] # VLM
    
    with open(caminho, 'r', encoding='utf-8') as f:
        conteudo = f.read()

    busca = re.findall(rf"{METRICA}\s*[:=]\s*([0-9.]+)", conteudo)
    if busca:
        dados[vlm][llm].append(float(busca[-1]))

tabela = []

# T-Student critical value for 95% confidence and 4 degrees of freedom (5 folds - 1)
# You can also use stats.t.ppf(0.975, df=4)
t_value = stats.t.ppf(0.975, df=4)

for vlm, backbones in sorted(dados.items()):
    for backbone, valores_metablock in sorted(backbones.items()):
        
        if backbone not in BASELINES:
            continue
            
        valores_baseline = BASELINES[backbone][DATASET_ALVO].get(METRICA, [])
        
        if not valores_baseline or len(valores_baseline) != len(valores_metablock):
            continue
            
        # Calcula a diferença (Metablock - Baseline)
        diferencas = np.array(valores_metablock) - np.array(valores_baseline)
        
        # Média das diferenças
        media_dif = np.mean(diferencas)
        
        # Erro padrão da diferença
        std_dif = np.std(diferencas, ddof=1) # Desvio padrão amostral
        se_dif = std_dif / np.sqrt(len(diferencas))
        
        # Intervalo de Confiança Pareado
        ic_margem = t_value * se_dif
        ic_inferior = media_dif - ic_margem
        ic_superior = media_dif + ic_margem
        
        # Formata os dados
        ic_str = f"[{ic_inferior:+.4f}, {ic_superior:+.4f}]"
        melhorou = "Sim" if ic_inferior > 0 else "Inconclusivo (Cruza Zero)"
        
        tabela.append({
            "VLLM": vlm,
            "Backbone": backbone,
            "Média Dif.": round(media_dif, 4),
            "Margem (IC 95%)": round(ic_margem, 4),
            "Intervalo de Confiança (Metablock - Baseline)": ic_str,
            "Significativo (IC > 0)": melhorou
        })

df = pd.DataFrame(tabela)

caminho_csv = os.path.join(base_dir, f"tabela_ic_diferenca_{DATASET_ALVO}.csv")
df.to_csv(caminho_csv, index=False)

print(f"--- INTERVALO DE CONFIANÇA PAREADO ({DATASET_ALVO.upper()}) ---")
print("O intervalo mostra o quanto o Metablock foi MELHOR (ou pior) que o Baseline.")
print("Se o intervalo cruzar o ZERO (ex: -0.01 a +0.05), a diferença estatística é inconclusiva.")
print("Se for estritamente positivo (ex: +0.02 a +0.10), o Metablock é ESTATISTICAMENTE MELHOR a 95% de confiança!\n")
print(df.to_string(index=False))
print(f"\nSalvo com sucesso em: {caminho_csv}")
