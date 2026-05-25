import numpy as np
from dataclasses import dataclass
import bisect

@dataclass
class posi:
    valor: int # Valor de custo para chegar nessa posicao
    valor_total: int # O valor gato para chegar até ela
    eixo: tuple # A posição (x,y) na matriz
    visitada: int # Se o nó já foi visitado ou não
    custo: int # Para quando for calcular a distância euclidiana

# def fitness(posi1, posi2, final): # Com calculo da distância euclidiana
#     som = 0
#     som = (posi2.eixo[0] - final[0])**2 + (posi2.eixo[1] - final[1])**2
#     posi2.custo = posi2.valor + posi1.custo
#     posi2.valor_total = posi2.custo + som**0.5

def fitness(posi1, posi2, final): # Sem o calculo da distancia euclidiana
    posi2.valor_total = posi2.valor + posi1.valor_total

def preenche(matriz): # Tranforma cada posicao em um objeto "posi"
    matriz_resul = np.empty(matriz.shape, dtype=object)
    for i in range(matriz.shape[0]):
        for j in range(matriz.shape[1]):
            if matriz[i,j] == "I" or matriz[i,j] == "F":
                if matriz[i,j] == "I":
                    inicial = (i,j)
                    novo = posi(-2, 0, (i,j), 0, 0)
                else:
                    final = (i,j)
                    novo = posi(-1, 0, (i,j), 0, 0)
            elif matriz[i,j] == "*":
                novo = posi(None, 0, (i,j), 0, 0)
            else:
                novo = posi(int(matriz[i,j]), 0, (i,j), 0, 0)
            matriz_resul[i,j] = novo
    return matriz_resul, inicial, final

def partida(matriz, aberta, expandida, inicial, final):
    linha, coluna = inicial
    aux = matriz[inicial]
    if linha + 1 < matriz.shape[0]: # A sequencia de if's irá verificar cada uma das 4 posições ao redor, vendo se é possivel ou não ir para lá, basicamente fazem a mesma coisa só mudam a posicao
        if matriz[linha + 1, coluna].valor != None and matriz[linha + 1, coluna].visitada == 0 and matriz[linha + 1, coluna] not in aberta: # Verifica se a posicao existe, não foi visitada e não foi inserida na lista
            fitness(aux, matriz[linha + 1, coluna], final)
            if matriz[linha + 1, coluna].valor == -1: # Caso a posicao seja a final já imprime tudo
                print("Caminho: ")
                for i in range(len(expandida)):
                    print(expandida[i].eixo, end=", ")
                print(f"\nCusto: {expandida[-1].valor_total}")
                return True # Aqui é pra parar o while da função "percorre"
            bisect.insort(aberta, matriz[linha + 1, coluna], key=lambda j: j.valor_total)
    if coluna + 1 < matriz.shape[1]:
        if matriz[linha, coluna + 1].valor != None and matriz[linha, coluna + 1].visitada == 0 and matriz[linha, coluna + 1] not in aberta:
            fitness(aux, matriz[linha, coluna + 1], final)
            if matriz[linha, coluna + 1].valor == -1: 
                print("Caminho: ")
                for i in range(len(expandida)):
                    print(expandida[i].eixo, end=", ")
                print(f"\nCusto: {expandida[-1].valor_total}")
                return True
            bisect.insort(aberta, matriz[linha, coluna + 1], key=lambda j: j.valor_total)
    if linha - 1 >= 0:
        if matriz[linha -1, coluna].valor != None and matriz[linha -1, coluna].visitada == 0 and matriz[linha -1, coluna] not in aberta:
            fitness(aux, matriz[linha -1, coluna], final)
            if matriz[linha -1, coluna].valor == -1:
                print("Caminho: ")
                for i in range(len(expandida)):
                    print(expandida[i].eixo, end=", ")
                print(f"\nCusto: {expandida[-1].valor_total}")
                return True
            bisect.insort(aberta, matriz[linha -1, coluna], key=lambda j: j.valor_total)
    if coluna - 1 >= 0:
        if matriz[linha, coluna - 1].valor != None and matriz[linha, coluna - 1].visitada == 0 and matriz[linha, coluna - 1] not in aberta:
            fitness(aux, matriz[linha, coluna - 1], final)
            if matriz[linha, coluna - 1].valor == -1:
                print("Caminho: ")
                for i in range(len(expandida)):
                    print(expandida[i].eixo, end=", ")
                print(f"\nCusto: {expandida[-1].valor_total}")
                return True
            bisect.insort(aberta, matriz[linha, coluna - 1], key=lambda j: j.valor_total)
    return False
def percorre(matriz, aberta, expandida, inicial, final):
    if inicial != final:
        while(len(aberta) > 0): # Enquanto tiver posição pra olhar, trabalhe
            atual = aberta.pop(0) # Remove da lista
            atual.visitada = 1 # Diz que foi visitada
            expandida.append(atual) # Insere da lista de percorridas
            check = partida(matriz, aberta, expandida, atual.eixo, final) # Insere as outras possiveis posições na lista "aberta"
            if check == True: # Caso já tenha chego no destino
                break
    else:
        return "O inicio e o final sao iguais voce chegou ao seu destino sem esforco"

def main():
    matriz = np.array([
    ['I', '2', '1',  '1', '1', '2'],
    ['1', '*', '2',  '*', '3', '1'],
    ['1', '4', '15', '*', '*', '1'],
    ['2', '*', '15', '*', '4', '1'],
    ['*', '*', '2',  '2', '9', '2'],
    ['*', '*', '*',  '*', 'F', '1']
])
    inicial = None
    final = None
    aberta = []
    expandida = []

    matriz, inicial, final = preenche(matriz)

    aux_inicial = matriz[inicial]
    aux_inicial.visitada = 1
    expandida.append(aux_inicial)

    partida(matriz, aberta, expandida, inicial, final)
    percorre(matriz, aberta, expandida, inicial, final)

main()