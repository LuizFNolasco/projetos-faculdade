import pandas as pd
from pathlib import Path
import os
import glob
import csv

pasta = "MICCAI" #AUGSBURG ou MICCAI
caminho = Path(f"../data/{pasta}")

df = None
cont = 0

padrao = None
arquivo = None

try:
    
    if not caminho.is_dir():
        raise FileNotFoundError("Erro, diretorio nao encontrado")

    if pasta == "AUGSBURG":
        diagnosticos = ['BOD', 'AGE', 'AMD', 'SON', 'AGU', 'BHD', 'BND']
        df = pd.DataFrame(columns=["patient", "img_id", "diagnostic"])

        padrao = os.path.join(caminho, "*.jpg")
        arquivo = glob.glob(padrao) + glob.glob(os.path.join(caminho, "*.JPG"))
        
        nomes = [os.path.basename(arq) for arq in arquivo]

        for i in nomes:
            df.loc[cont, "patient"] = "pat" + i.split("_")[0]
            df.loc[cont, "img_id"] = i
            df.loc[cont, "diagnostic"] = i.split("_")[1]
            df.loc[cont, "diagnostic_number"] = diagnosticos.index(i.split("_")[1])

            cont += 1

    elif pasta == "MICCAI":
        diagnosticos = ['NDBT','ACHD']
        df = pd.DataFrame(columns=["patient", "img_id", "diagnostic"])
        
        padrao = os.path.join(caminho, "*.tif")
        arquivo = glob.glob(padrao) + glob.glob(os.path.join(caminho, "*.TIF"))

        nomes = [os.path.basename(arq) for arq in arquivo]

        for i in nomes:
            df.loc[cont, "patient"] = i.split("_")[0]
            df.loc[cont, "img_id"] = i
            df.loc[cont, "diagnostic"] = i.split("_")[2].split(".")[0]
            df.loc[cont, "diagnostic_number"] = diagnosticos.index(i.split("_")[2].split(".")[0])

            cont += 1

    with open(f"{pasta}_metadata.csv", "w", newline= "") as file:
        escrever = csv.writer(file, delimiter=",")

        escrever.writerow(df.columns)
        escrever.writerows(df.values)

except Exception as erro:
    print("Rapaz, acho que deu erro")