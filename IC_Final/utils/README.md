# Códigos úteis para tarefas gerais

Este repositório contém códigos úteis para tarefas gerais, como cálculo de métricas, manipulação de arquivos, criação de pastas, etc.

A ideia é que este repositório seja um local para armazenar códigos que podem ser úteis em diferentes projetos, evitando a necessidade de reescrever códigos que já foram feitos anteriormente.

### Estrutura do repositório

A estrutura do repositório é bem simples, cada arquivo `.py` contém uma série de funções relacionadas a um tema específico. Por exemplo, o arquivo `seg_metrics.py` contém funções para calcular métricas de segmentação de imagens. 

Dessa forma, para encontrar códigos relacionados a um tema específico, basta procurar pelo arquivo correspondente (ver índice abaixo). Se você não encontrar o que procura, sinta-se à vontade para abrir uma issue solicitando a adição de um novo arquivo e/ou função.

### Índice

- `seg_metrics.py`: funções para cálculo de métricas de segmentação de imagens (por exemplo, IoU, Dice, etc).
- `stats.py`: funções para cálculo de estatísticas (por exemplo, teste estatístico, medias, etc).
- `plots.py`: funções para plotagem de gráficos.
- `optimizers.py`: funções para otimização de hiperparâmetros.
- `signals.py`: funções para processamento de sinais.
- `myfiles.py`: funções para manipulação de arquivos.

### Contribuições

Se você deseja contribuir com códigos úteis para tarefas gerais, sinta-se à vontade para tal. Basta seguir os passos abaixo:

1. Identifique um tema que você deseja adicionar ao repositório (por exemplo, cálculo de métricas de classificação, manipulação de arquivos, etc). Se ele já existe, basta incluir a função no arquivo correspondente. Caso contrário, crie um novo arquivo `.py` com o nome do tema.

2. As funções devem ser bem documentadas, seguindo o padrão que já está sendo utilizado nos arquivos existentes.

3. Se foir criado um outro arquivo `.py`, adicione-o ao índice acima.

4. Se utilizou alguma biblioteca externa, adicione-a ao arquivo `requirements.txt`.


### Dependências

As dependências do repositório estão listadas no arquivo `requirements.txt`. Para instalá-las, basta executar o comando abaixo:

```bash 
pip install -r requirements.txt
```





