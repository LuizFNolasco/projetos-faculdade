import pandas as pd
import sqlite3
import warnings
from pathlib import Path
from functools import reduce

warnings.filterwarnings('ignore')

def parse_br_float(series):
    return series.astype(str).str.replace('.', '', regex=False).str.replace(',', '.', regex=False).astype(float)

print("Iniciando Pipeline de ETL (Bronze -> Silver -> Gold)...")

base_dir = Path('datasets')
db_path = 'DW_Banestes.db'

with sqlite3.connect(db_path) as conn:
    
    print("Dim_Fundo")
    df_fundos = pd.read_csv(base_dir / 'fundos_anbima_fake.csv', sep=';', encoding='latin1', 
                            usecols=['Código ANBIMA', 'CNPJ do Fundo', 'Nome Comercial', 'Nível 1 Categoria'])
    df_fundos.columns = ['ID_Fundo', 'CNPJ', 'Nome', 'Categoria']

    dim_fundo_bcri = pd.DataFrame({
        'ID_Fundo': ['BCRI11'],
        'CNPJ': ['17438291000194'],
        'Nome': ['BANESTES RECEBÍVEIS IMOBILIÁRIOS FII'],
        'Categoria': ['Fundo Imobiliário']
    })
    dim_fundo = pd.concat([df_fundos, dim_fundo_bcri], ignore_index=True)
    dim_fundo.to_sql('Dim_Fundo', conn, if_exists='replace', index=False)

    print("Dim_Cliente")
    dim_cliente = pd.read_csv(base_dir / 'clientes_fake.csv', sep=';', usecols=['ID_Cliente', 'Nome', 'Perfil_Risco'])
    dim_cliente.to_sql('Dim_Cliente', conn, if_exists='replace', index=False)

    print("Fato_Movimentacao")
    fato_movimentacao = pd.read_csv(base_dir / 'movimentacoes_fake.csv', sep=';', usecols=['Data', 'ID_Cliente', 'Codigo_ANBIMA', 'Tipo', 'Valor'])
    fato_movimentacao.columns = ['Data', 'ID_Cliente', 'ID_Fundo', 'Tipo', 'Valor']
    
    datas_mov = pd.to_datetime(fato_movimentacao['Data'])
    fato_movimentacao['Data'] = datas_mov.dt.strftime('%Y-%m-%d')
    fato_movimentacao['Mes_Ano'] = datas_mov.dt.strftime('%Y-%m')
    fato_movimentacao.to_sql('Fato_Movimentacao', conn, if_exists='replace', index=False)

    print("Processando Fato_Indicadores_Macro")
    df_ipca = pd.read_csv(base_dir / 'IPCA_mensal.csv', sep=';', encoding='latin1', names=['Data', 'IPCA'], header=0)
    df_ipca['IPCA'] = parse_br_float(df_ipca['IPCA'])
    
    df_selic = pd.read_csv(base_dir / 'SELIC_CDI.csv', sep=';', encoding='latin1', names=['Data', 'Selic', 'CDI'], header=0)
    df_selic['Selic'] = parse_br_float(df_selic['Selic'])
    df_selic['CDI'] = parse_br_float(df_selic['CDI'])
    
    df_cambio = pd.read_csv(base_dir / 'cambio1.csv', sep=';', encoding='latin1', usecols=[0, 1], names=['Data', 'Dolar'], header=0)
    df_cambio['Dolar'] = parse_br_float(df_cambio['Dolar'])
    
    df_pib = pd.read_csv(base_dir / 'PIB_BC.csv', sep=';', encoding='latin1', usecols=[0, 1], names=['Data', 'PIB'], header=0)
    df_pib['PIB'] = parse_br_float(df_pib['PIB'])

    dfs_macro = [df_ipca, df_selic, df_cambio, df_pib]
    df_macro = reduce(lambda left, right: pd.merge(left, right, on='Data', how='outer'), dfs_macro)

    df_macro['Data'] = pd.to_datetime(df_macro['Data'], format='%m/%Y', errors='coerce').dt.strftime('%Y-%m-01')

    df_yahoo = pd.read_csv(base_dir / 'dados_yahoo_finance.csv')
    df_yahoo['Date'] = pd.to_datetime(df_yahoo['Date']).dt.strftime('%Y-%m-01')
    df_yahoo_ibov = df_yahoo[['Date', '^BVSP']].rename(columns={'Date': 'Data', '^BVSP': 'Ibovespa'}).dropna()

    df_macro = df_macro.merge(df_yahoo_ibov, on='Data', how='outer').dropna(subset=['Data'])
    df_macro['Mes_Ano'] = pd.to_datetime(df_macro['Data']).dt.strftime('%Y-%m')
    df_macro.to_sql('Fato_Indicadores_Macro', conn, if_exists='replace', index=False)

    print("Fato_Cotacao_Mercado")
    fato_cotacao = df_yahoo[['Date', 'BCRI11.SA']].dropna().copy()
    fato_cotacao.columns = ['Data', 'Valor_Cota']
    fato_cotacao.insert(2, 'ID_Fundo', 'BCRI11')
    fato_cotacao['Mes_Ano'] = pd.to_datetime(fato_cotacao['Data']).dt.strftime('%Y-%m')
    fato_cotacao.to_sql('Fato_Cotacao_Mercado', conn, if_exists='replace', index=False)

    print("Dim_Tempo")
    todas_datas = pd.to_datetime(pd.concat([
        fato_movimentacao['Data'], 
        df_macro['Data'], 
        fato_cotacao['Data']
    ]).dropna().unique())
    
    dim_tempo = pd.DataFrame({
        'Data': todas_datas.strftime('%Y-%m-%d'),
        'Ano': todas_datas.year,
        'Mes': todas_datas.month,
        'Trimestre': todas_datas.quarter,
        'Mes_Ano': todas_datas.strftime('%Y-%m')
    })
    dim_tempo.to_sql('Dim_Tempo', conn, if_exists='replace', index=False)

print("Cabou")