import os
import sys
import re
import glob
import numpy as np
import pandas as pd
from scipy.stats import wilcoxon
from collections import defaultdict

sys.path.append(os.path.abspath(".."))
from baselines_extraidos import BASELINES

# --- CONFIGURAÇÃO ---
DATASET_ALVO = "miccai"
PATHS = {
    "augsburg_multi": "benchmarks/augsburg/results_MultiClass/opt_adam_early_stop_loss",
    "augsburg_binario": "benchmarks/augsburg/results_Binario/opt_adam_early_stop_loss",
    "miccai": "benchmarks_miccai/miccai/results/opt_adam_early_stop_loss"
}
base_dir = PATHS[DATASET_ALVO]
arquivo = "metrics.txt"

# Usaremos a Balanced Accuracy para a tabela (você pode alterar para Accuracy se quiser)
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

for vlm, backbones in sorted(dados.items()):
    for backbone, valores_metablock in sorted(backbones.items()):
        
        if backbone not in BASELINES:
            continue
            
        valores_baseline = BASELINES[backbone][DATASET_ALVO].get(METRICA, [])
        
        if not valores_baseline or len(valores_baseline) != len(valores_metablock):
            continue
            
        # Executa o teste de Wilcoxon diretamente (sem passar pelo Friedman)
        try:
            estatistica, p_value = wilcoxon(valores_baseline, valores_metablock)
        except ValueError as e:
            if 'zero for all elements' in str(e):
                estatistica = 0.0
                p_value = 1.0
            else:
                raise e
                
        rejeita_h0 = "Sim" if p_value < 0.05 else "Não"
        
        tabela.append({
            "VLLM": vlm,
            "Backbone": backbone,
            "Estatística": round(estatistica, 1),
            "p-value": round(p_value, 6),
            "Rejeita H0?": rejeita_h0
        })

df = pd.DataFrame(tabela)

caminho_csv = os.path.join(base_dir, f"tabela_wilcoxon_{DATASET_ALVO}.csv")
df.to_csv(caminho_csv, index=False)

print(f"--- TABELA DE WILCOXON ({DATASET_ALVO.upper()}) ---")
print(df.to_string(index=False))
print(f"\nSalvo com sucesso em: {caminho_csv}")
