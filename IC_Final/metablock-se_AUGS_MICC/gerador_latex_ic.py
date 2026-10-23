import pandas as pd
import sys

def csv_to_latex(csv_file, dataset_name):
    df = pd.read_csv(csv_file)
    
    # Model renaming for better display
    vlm_map = {
        "gemma3:4b": "Gemma-3 (4B)",
        "gemma3:12b": "Gemma-3 (12B)",
        "gemma3:27b": "Gemma-3 (27B)",
        "llama4:16x17b": "Llama-4 (16x17B)",
        "qwen2.5vl:3b": "Qwen-2.5-VL (3B)",
        "qwen2.5vl:7b": "Qwen-2.5-VL (7B)",
        "qwen2.5vl:32b": "Qwen-2.5-VL (32B)",
        "llava:7b": "LLaVA (7B)",
        "llava:13b": "LLaVA (13B)",
        "llava:34b": "LLaVA (34B)"
    }
    
    bb_map = {
        "efficientnet-b4": "EfficientNet-B4",
        "caformer_s18": "CAFormer-S18",
        "mobilenet": "MobileNet",
        "resnet-50": "ResNet-50"
    }
    
    # Sort by VLM and Backbone order
    bb_order = ["efficientnet-b4", "caformer_s18", "mobilenet", "resnet-50"]
    vlm_order = list(vlm_map.keys())
    
    df['VLLM'] = pd.Categorical(df['VLLM'], categories=vlm_order, ordered=True)
    df['Backbone'] = pd.Categorical(df['Backbone'], categories=bb_order, ordered=True)
    df = df.sort_values(['VLLM', 'Backbone'])
    
    latex = []
    latex.append("\\begin{table}[H]")
    latex.append("\\centering")
    latex.append(f"\\caption{{Intervalo de Confiança Pareado a 95\\% para a base \\textbf{{{dataset_name}}}.}}")
    latex.append(f"\\label{{tab:ic_{dataset_name.lower().replace(' ', '_')}}}")
    latex.append("\\resizebox{\\columnwidth}{!}{%")
    latex.append("\\begin{tabular}{|c|c|c|c|c|c|}")
    latex.append("\\hline")
    latex.append("\\textbf{VLLM} & \\textbf{Backbone} & \\textbf{Média Dif.} & \\textbf{Margem (IC 95\\%)} & \\textbf{Intervalo de Confiança} & \\textbf{Significativo (IC > 0)} \\\\ \\hline")
    
    current_vlm = None
    for _, row in df.iterrows():
        vlm = vlm_map.get(row['VLLM'], row['VLLM'])
        bb = bb_map.get(row['Backbone'], row['Backbone'])
        
        mdif = f"{row['Média Dif.']:.4f}"
        if not mdif.startswith('-'): mdif = "+" + mdif
        
        marg = f"{row['Margem (IC 95%)']:.4f}"
        ic = row['Intervalo de Confiança (Metablock - Baseline)']
        sig = "Sim" if row['Significativo (IC > 0)'] == "Sim" else "Inconclusivo"
        
        if vlm != current_vlm:
            col1 = f"\\multirow{{4}}{{*}}{{{vlm}}}"
            current_vlm = vlm
        else:
            col1 = ""
            
        latex.append(f"{col1} & {bb} & {mdif} & {marg} & {ic} & {sig} \\\\")
        
        if bb == "ResNet-50":
            latex.append("\\hline")
            
    latex.append("\\end{tabular}%")
    latex.append("}")
    latex.append("\\end{table}")
    
    print("\n" + "\n".join(latex) + "\n")

csv_miccai = "benchmarks_miccai/miccai/results/opt_adam_early_stop_loss/tabela_ic_diferenca_miccai.csv"
csv_augsburg = "benchmarks/augsburg/results_Binario/opt_adam_early_stop_loss/tabela_ic_diferenca_augsburg_binario.csv"

csv_to_latex(csv_miccai, "Miccai")
csv_to_latex(csv_augsburg, "Augsburg Binário")
