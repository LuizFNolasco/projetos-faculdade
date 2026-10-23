import numpy as np

def main():
    mat = np.genfromtxt("avg_mat.csv", delimiter=",", dtype=str)
    algo = np.copy(mat[1:, 0])
    mat = np.copy(mat[1:, 1:]).astype(float)

    std = np.genfromtxt("avg_std.csv", delimiter=",")
    std = np.copy(std[1:, 1:])

    parametro = np.genfromtxt("params.csv", delimiter=",", dtype=str)
    header_resul = list(parametro[0, :])
    header_resul[0] = "Algorithms"

    print(header_resul)
    coluna = np.copy(parametro[1:, 1])
    
    ideal_posi_mat, ideal_neg_mat = solucoes_ideais(mat, coluna[0])
    ideal_posi_std, ideal_neg_std = solucoes_ideais(std, coluna[1])

    euclid_posi_mat = dist_euclid(mat, ideal_posi_mat)
    euclid_neg_mat = dist_euclid(mat, ideal_neg_mat)
    euclid_posi_std = dist_euclid(std, ideal_posi_std)
    euclid_neg_std = dist_euclid(std, ideal_neg_std)

    mat_prox_relat = coef_prox_relativa(euclid_posi_mat, euclid_neg_mat)
    std_prox_relat = coef_prox_relativa(euclid_posi_std, euclid_neg_std)

    matriz_coef_relat = np.column_stack((mat_prox_relat * float(coluna[2]), std_prox_relat * float(coluna[3])))

    ideal_posi_glob, ideal_neg_glob = solucoes_ideais(matriz_coef_relat, "B")

    euclid_posi_glob = dist_euclid(matriz_coef_relat, ideal_posi_glob) 
    euclid_neg_glob = dist_euclid(matriz_coef_relat, ideal_neg_glob)

    prox_relat_glob = coef_prox_relativa(euclid_posi_glob, euclid_neg_glob)
    
    final = np.column_stack((algo, prox_relat_glob))

    cabecalho = ",".join(header_resul)

    np.savetxt("resultados.csv", final, delimiter=",", fmt="%s", header=cabecalho, comments="")

    fig, ax = plt.subplots()
    ax.plot(final)

def solucoes_ideais(orig, tipo): #No problema dado só foi avaliado o custo
    ideal_posi = np.zeros(orig.shape[1])
    ideal_neg = np.zeros(orig.shape[1])
    if tipo == "C":
        for i in range(orig.shape[1]):
            ideal_posi[i] = np.min(orig[:, i])
            ideal_neg[i] = np.max(orig[:, i])
    else:
        for i in range(orig.shape[1]):
            ideal_posi[i] = np.max(orig[:, i])
            ideal_neg[i] = np.min(orig[:, i])

    return ideal_posi, ideal_neg
    
def dist_euclid(orig, ideal):
    result = np.zeros(orig.shape[0])
    for j in range(orig.shape[0]):
        cont = 0
        for i in range(orig.shape[1]):
            cont += (ideal[i] - orig[j,i])**2
        result[j] = cont ** 0.5

    return result

def coef_prox_relativa(eucli_posi, eucli_neg):
    arr_relat = np.zeros(len(eucli_posi))
    if len( eucli_neg) == len(eucli_posi):  
        for i in range(len(eucli_posi)):
            arr_relat[i] = (eucli_neg[i])/(eucli_posi[i] + eucli_neg[i])

    return arr_relat

main()