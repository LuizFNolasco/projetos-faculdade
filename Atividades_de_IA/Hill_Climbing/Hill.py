import numpy as np
from dataclasses import dataclass
import random
import matplotlib.pyplot as plt

CUSTO = 100
TEMPO = 90
RODADAS = 100

@dataclass
class cand:
    nome: str
    custo: int
    tempo: int

def fitness(lista):
    cont_custo = 0
    cont_tempo = 0
    for i in lista:
        if (cont_custo + i.custo) >= CUSTO or (cont_tempo + i.tempo) >= TEMPO:
            return -1
        cont_custo += i.custo
        cont_tempo += i.tempo
    return cont_custo + cont_tempo

def verifica(resultado, candidatos):
    lista_aux = resultado.copy()
    
    lista_aux.append(random.choice(candidatos))
    if fitness(lista_aux) == -1:
        lista_aux.pop()

    if fitness(resultado) >= fitness(lista_aux):
        return resultado
    else:
        return lista_aux

def plotar(resultado, candidatos):
    fig, ax = plt.subplots()
    aparece = [0] * len(candidatos)
    quem = []

    for i in resultado:
        aux = 0
        for j in candidatos:
            if i == j:
                aparece[aux] += 1
            aux += 1


    for i in candidatos:
        quem.append(i.nome)

    ax.bar(quem, aparece)
    ax.set_title("Hill Climbing")
    ax.set_xlabel("Candidatos")
    ax.set_ylabel("Vezes que apareceu")

    plt.tight_layout()
    plt.show()

def main():
    lista_cand = [cand("A", 3, 2), cand("B", 2, 4), cand("C", 4, 3)]
    resultado_pri = []

    for i in range(RODADAS):
        resultado_pri = verifica(resultado_pri, lista_cand)

    plotar(resultado_pri, lista_cand)

main()