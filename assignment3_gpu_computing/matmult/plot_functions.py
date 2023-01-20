import io_functions
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
sns.set(rc={'figure.figsize':(9,6)})
sns.set_style("whitegrid")
# sns.set(rc={'figure.figsize':(11.7,8.27)})

def plot_block_experiment(experiment: str):
    df = io_functions.read_experiment(experiment)
    version = "blk_omp"
    omp_df = df.query('version==@version')
    print(df)
    omp_df.iloc[:, 1:-3] = omp_df.iloc[:, 1:-3].astype(float)
    fig = sns.lineplot(data=omp_df, x='block_size', y='performance')

    df_max = omp_df.query("(performance == performance.max())")[["block_size", "performance"]].reset_index(drop=True)
    fig = sns.scatterplot(ax=fig, data=df_max, x='block_size', y='performance', color='red')
    fig.text(x=df_max['block_size'] - 5, y=df_max['performance']+5, s=f"Optimal Blocksize: {df_max.at[0, 'block_size']}")
    
    fig.set_title(f"{version}, 2048x2048 matrices")
    fig.set_xlabel("Block Size")
    fig.set_ylabel("Performance (MFlops/s)")
    print(df_max.at[0, 'block_size'])
    plt.savefig(f'{version}_block_perform.png')

def plot_ex2(exp_name: str):
    df = io_functions.read_experiment(exp_name)
    # df.iloc[:, 1:-1] = df.iloc[:, 1:-1].astype(float)
    print(df)
    df.to_csv('test_ex2.csv')
    # print(df.dtypes)
    # fig = sns.lineplot(data=df, x='block_size', y='performance', hue='version')
    # plt.savefig('test_ex2.pdf')

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-q', type=int, help='Question to plot')
    parser.add_argument('--expname', type=str, help='Name of the experiment to plot')
    # plot_block_experiment("blk_size_500_20230118_134540")

    args = parser.parse_args()

    if args.q == 1:
        plot_block_experiment(args.expname)
    if args.q == 2:
        plot_ex2(args.expname)
    
