import statistics
from baselines_extraidos import BASELINES

# Escolha o dataset que quer calcular as médias do baseline
DATASET_ALVO = "augsburg_multi"
ARQUIVO_SAIDA = f"resultado_baselines_medias_{DATASET_ALVO}.txt"

with open(ARQUIVO_SAIDA, "w", encoding="utf-8") as f:
    f.write(f"=== MÉDIAS E DESVIO PADRÃO DOS BASELINES ({DATASET_ALVO.upper()}) ===\n\n")

    for modelo, datasets in BASELINES.items():
        if DATASET_ALVO in datasets:
            metricas = datasets[DATASET_ALVO]
            
            # Função para calcular
            def calc_stats(valores):
                if not valores: return 0.0, 0.0
                if len(valores) == 1: return valores[0], 0.0
                return statistics.mean(valores), statistics.stdev(valores)
                
            loss_media, loss_std = calc_stats(metricas.get("Loss", []))
            acc_media, acc_std = calc_stats(metricas.get("Accuracy", []))
            bacc_media, bacc_std = calc_stats(metricas.get("Balanced accuracy", []))
            
            texto = (
                f"Modelo: {modelo}\n"
                f"Loss: {loss_media:.4f} ± {loss_std:.4f}\n"
                f"Acc : {acc_media:.4f} ± {acc_std:.4f}\n"
                f"BAcc: {bacc_media:.4f} ± {bacc_std:.4f}\n"
                + "-" * 40 + "\n"
            )
            print(texto, end="")
            f.write(texto)

print(f"\nSalvo com sucesso em: {ARQUIVO_SAIDA}")

