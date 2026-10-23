import sqlite3
import pandas as pd
from pathlib import Path

output_dir = Path('dados_gold')
output_dir.mkdir(parents=True, exist_ok=True)

db_path = 'DW_Banestes.db'
print(f"Conectando ao banco de dados: {db_path}...")

with sqlite3.connect(db_path) as conn:
    
    query_tabelas = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';"
    
    tabelas = pd.read_sql_query(query_tabelas, conn)['name'].tolist()
    
    print(f"{len(tabelas)} tabelas na Camada Gold. Iniciando exportação para '{output_dir}/'...\n")
    
    for tabela in tabelas:
        output_path = output_dir / f"{tabela}.csv"
        
        df = pd.read_sql_query(f"SELECT * FROM {tabela}", conn)
        
        df.to_csv(output_path, index=False, encoding='utf-8', decimal=',')
        
        print(f" -> Arquivo gerado: {output_path.name} ({len(df)} linhas extraídas)")