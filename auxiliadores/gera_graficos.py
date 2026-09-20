import pandas as pd
import matplotlib.pyplot as plt

# ==========================================
# 1. CARREGANDO OS DADOS
# ==========================================

excel_file = 'tabelas_tempos.xlsx'

df = pd.read_excel(excel_file, sheet_name='parte2')

# Preenche os nomes dos problemas para baixo
df['Opção do Menu'] = df['Opção do Menu'].ffill()

print("Dados carregados:")
print(df)


# ==========================================
# 2. GRÁFICO DE SPEEDUP x THREADS
# ==========================================

plt.figure(figsize=(10, 6))

for problema in df['Opção do Menu'].unique():

    subset = df[df['Opção do Menu'] == problema]

    plt.plot(
        subset['Threads (p)'],
        subset['Speedup (Sp​)'],
        marker='o',
        linewidth=2,
        markersize=6,
        label=problema
    )

plt.title(
    'Speedup × Número de Threads',
    fontsize=14,
    fontweight='bold'
)

plt.xlabel(
    'Número de Threads',
    fontsize=11
)

plt.ylabel(
    'Speedup',
    fontsize=11
)

plt.xticks([1, 2, 4, 8])

# Foca a escala nos valores reais
plt.ylim(0.9, 2.0)

plt.grid(
    True,
    linestyle='--',
    alpha=0.5
)

plt.legend()

plt.tight_layout()

# Salva o primeiro gráfico
plt.savefig(
    'grafico_speedup_threads.png',
    dpi=300,
    bbox_inches='tight'
)

plt.show()


# ==========================================
# 3. GRÁFICO DE EFICIÊNCIA x THREADS
# ==========================================

plt.figure(figsize=(10, 6))

for problema in df['Opção do Menu'].unique():

    subset = df[df['Opção do Menu'] == problema]

    # Converte eficiência de decimal para porcentagem
    eficiencia = subset['Eficiência (Ep​)'] * 100

    plt.plot(
        subset['Threads (p)'],
        eficiencia,
        marker='o',
        linewidth=2,
        markersize=6,
        label=problema
    )

plt.title(
    'Eficiência × Número de Threads',
    fontsize=14,
    fontweight='bold'
)

plt.xlabel(
    'Número de Threads',
    fontsize=11
)

plt.ylabel(
    'Eficiência (%)',
    fontsize=11
)

plt.xticks([1, 2, 4, 8])

# Como a maior eficiência é 100%
plt.ylim(0, 105)

plt.grid(
    True,
    linestyle='--',
    alpha=0.5
)

plt.legend()

plt.tight_layout()

# Salva o segundo gráfico
plt.savefig(
    'grafico_eficiencia_threads.png',
    dpi=300,
    bbox_inches='tight'
)

plt.show()


print("\nGráficos gerados com sucesso:")
print("- grafico_speedup_threads.png")
print("- grafico_eficiencia_threads.png")