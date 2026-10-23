import pandas as pd
import numpy as np
from pathlib import Path

base_dir = Path("dados_brutos")
base_dir.mkdir(exist_ok=True)

print("Lendo catálogo de fundos...")
df_fundos = pd.read_csv(base_dir / "fundos_anbima_fake.csv", sep=";", encoding="latin1")
codigos_fundos = df_fundos['Código ANBIMA'].dropna().unique()

print("Gerando dimensão de clientes (Vetorizado)...")
n_clientes = 500

start_cli = pd.to_datetime('2019-01-01').value // 10**9
end_cli = pd.to_datetime('2021-12-31').value // 10**9

df_clientes = pd.DataFrame({
    "ID_Cliente": np.arange(1, n_clientes + 1),
    "Nome": [f"Cliente_{i}" for i in range(1, n_clientes + 1)],
    "Perfil_Risco": np.random.choice(["Conservador", "Moderado", "Arrojado"], size=n_clientes),
    "Data_Cadastro": pd.to_datetime(np.random.randint(start_cli, end_cli, n_clientes), unit='s').strftime("%Y-%m-%d")
})

df_clientes.to_csv(base_dir / "clientes_fake.csv", sep=";", index=False)
print(f"{n_clientes} clientes gerados e salvos.")

print("Gerando fatos de movimentação (Vetorizado)...")
n_movs = 5000

start_mov = pd.to_datetime('2021-01-01').value // 10**9
end_mov = pd.to_datetime('2024-12-31').value // 10**9

df_movimentacoes = pd.DataFrame({
    "Data": pd.to_datetime(np.random.randint(start_mov, end_mov, n_movs), unit='s').strftime("%Y-%m-%d"),
    "ID_Cliente": np.random.randint(1, n_clientes + 1, size=n_movs),
    "Codigo_ANBIMA": np.random.choice(codigos_fundos, size=n_movs),
    "Tipo": np.random.choice(["Aplicação", "Resgate"], p=[0.7, 0.3], size=n_movs),
    "Valor": np.round(np.random.uniform(500.0, 50000.0, size=n_movs), 2)
})

df_movimentacoes.to_csv(base_dir / "movimentacoes_fake.csv", sep=";", index=False)
print(f"{n_movs} movimentações geradas e salvas.")