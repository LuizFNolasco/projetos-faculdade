import os
import json
import re

def extract_metrics(notebook_path):
    if not os.path.exists(notebook_path):
        return None
        
    with open(notebook_path, 'r', encoding='utf-8') as f:
        nb = json.load(f)
        
    folds_best_metrics = {}
    current_fold = 1
    best_loss = float('inf')
    best_metrics = {'Loss': None, 'Accuracy': None, 'Balanced accuracy': None}
    
    for cell in nb.get('cells', []):
        if cell.get('cell_type') != 'code':
            continue
            
        for output in cell.get('outputs', []):
            if output.get('output_type') == 'stream':
                text = output.get('text', [])
                if isinstance(text, str):
                    text = [text]
                for line in text:
                    
                    # Detecta o início de um novo fold e avança o current_fold se necessário
                    if "--- Fim do FOLD" in line:
                        if best_metrics['Loss'] is not None:
                            folds_best_metrics[current_fold] = best_metrics.copy()
                            current_fold += 1
                            best_metrics = {'Loss': None, 'Accuracy': None, 'Balanced accuracy': None}
                            best_loss = float('inf')

                    # Tenta capturar a métrica por época e salva a melhor (usado no Augsburg)
                    match = re.search(r'Val Loss:\s*([0-9.]+),\s*Val Acc:\s*([0-9.]+),\s*(?:Val\s*)?Bal Acc:\s*([0-9.]+)', line)
                    if match:
                        loss = float(match.group(1))
                        acc = float(match.group(2))
                        bacc = float(match.group(3))
                        if loss < best_loss:
                            best_loss = loss
                            best_metrics = {'Loss': loss, 'Accuracy': acc, 'Balanced accuracy': bacc}
                
                    # Se tiver métricas "Final" impressas, a gente simplesmente sobrescreve no dicionário oficial!
                    # Isso funciona perfeitamente para o Miccai que imprime essas métricas soltas no final de tudo.
                    match_loss = re.search(r'Loss Final.*?FOLD\s*(\d+).*?:\s*([0-9.]+)', line)
                    if match_loss:
                        fold_num = int(match_loss.group(1))
                        if fold_num not in folds_best_metrics: folds_best_metrics[fold_num] = {}
                        folds_best_metrics[fold_num]['Loss'] = float(match_loss.group(2))
                        
                    match_acc = re.search(r'(?:Acur[aá]cia|Accuracy) Final.*?FOLD\s*(\d+).*?:\s*([0-9.]+)', line)
                    if match_acc:
                        fold_num = int(match_acc.group(1))
                        if fold_num not in folds_best_metrics: folds_best_metrics[fold_num] = {}
                        folds_best_metrics[fold_num]['Accuracy'] = float(match_acc.group(2))
                        
                    match_bacc = re.search(r'(?:Acur[aá]cia Balanceada|Balanced accuracy) Final.*?FOLD\s*(\d+).*?:\s*([0-9.]+)', line)
                    if match_bacc:
                        fold_num = int(match_bacc.group(1))
                        if fold_num not in folds_best_metrics: folds_best_metrics[fold_num] = {}
                        folds_best_metrics[fold_num]['Balanced accuracy'] = float(match_bacc.group(2))

    # Algum notebook pode ter faltado preencher um fold, garantimos que temos 5 folds ou o que der
    if not folds_best_metrics:
        return None

    final_dict = {'Loss': [], 'Accuracy': [], 'Balanced accuracy': []}
    for fold in sorted(folds_best_metrics.keys()):
        for metric in final_dict:
            # Caso não encontre a métrica (por algum motivo), coloca None
            final_dict[metric].append(folds_best_metrics[fold].get(metric, None))
            
    return final_dict

models = ["Efficientnet", "Resnet50", "Mobilenet", "Caformer"]
datasets = {
    "augsburg_multi": "Augsburg-Multi.ipynb",
    "augsburg_binario": "Augsburg-Binario.ipynb",
    "miccai": "Miccai.ipynb"
}

base_path = "/home/luiz_felipe/IC/Projetos/aux/"

all_results = {}

for model in models:
    model_lower = model.lower()
    if model_lower == "resnet50":
        model_lower = "resnet-50"
    elif model_lower == "efficientnet":
        model_lower = "efficientnet-b4"
    elif model_lower == "caformer":
        model_lower = "caformer_s18"
    # mobilenet stays mobilenet
        
    all_results[model_lower] = {}
    for ds_name, ds_file in datasets.items():
        nb_path = os.path.join(base_path, model, ds_file)
        if not os.path.exists(nb_path):
            nb_path = os.path.join(base_path, model, ds_file.replace("Augsburg", "Augsbug"))
        res = extract_metrics(nb_path)
        if res:
            all_results[model_lower][ds_name] = res
        else:
            print(f"Failed or missing: {nb_path}")

with open("baselines_extraidos.py", "w") as f:
    f.write("BASELINES = ")
    f.write(json.dumps(all_results, indent=4))
    f.write("\n")
    
print("Extraction complete. Wrote to baselines_extraidos.py")

