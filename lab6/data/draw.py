import pandas as pd
import matplotlib.pyplot as plt



def draw(s):
    df = pd.read_csv(s)
    plt.plot(df['NumThreads'], df['ExecutionTime'], marker='o')
    plt.title('Execution Time vs NumThreads')
    plt.xlabel('NumThreads')
    plt.ylabel('Execution Time')
    plt.grid(True)
    plt.savefig(f'drawing_{f.split(".")[0].split("_")[1]}.png', dpi=300)
    
csv_files = ['data_20000000.csv', 'data_10000000.csv', 'data_1000000.csv', 'data_100000.csv'   ]

csv_files.reverse()
for f in csv_files:
    draw(f)