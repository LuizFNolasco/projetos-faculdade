from datetime import datetime
import requests
import json

def main():
    
    sub = str(input("Digite o nome do subreddit: "))

    print("Lembre de usar '/' como separador e que a data deve ser no formato dd/mm/aaaa")
    
    data_inicial = str(input("Digite a data inicial: "))
    data_final = str(input("Digite a data final: "))

    data_inicial = datetime.strptime(data_inicial, "%d/%m/%Y")
    data_final = datetime.strptime(data_final, "%d/%m/%Y")

    data_inicial = int(data_inicial.timestamp())
    data_final = int(data_final.timestamp())

    repertir = int(input("Qual o número de palavras que deseja realizar o raking: "))

    site = f"https://api.pullpush.io/reddit/search/submission/?subreddit={sub}&after={data_inicial}&before={data_final}"

    resposta = requests.get(site)

    resposta_json = resposta.json()

    lista = []

    for i in resposta_json["data"]:
        lista += i.get["selftext", ""] 

    with open("ranking.json", "w") as arquivo:
        json.dump(resposta_json, arquivo, indent=4)


main()