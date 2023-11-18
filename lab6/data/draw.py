import pandas as pd
import matplotlib.pyplot as plt

# Load data from CSV
csv_files = ['data_20000000.csv', 'data_10000000.csv', 'data_1000000.csv', 'data_100000.csv'   ]

for f in csv_files:
    df = pd.read_csv(f)
    plt.plot(df['NumThreads'], df['ExecutionTime'], marker='o')
    plt.title('Execution Time vs NumThreads')
    plt.xlabel('NumThreads')
    plt.ylabel('Execution Time')
    plt.grid(True)
    plt.savefig(f'drawing_{f.split(".")[0].split("_")[1]}.png', dpi=300)