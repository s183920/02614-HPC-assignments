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
    omp_df = df.query('version=="blk_omp"')

    omp_df.iloc[:, 1:-1] = omp_df.iloc[:, 1:-1].astype(float)
    fig = sns.lineplot(data=omp_df, x='block_size', y='performance')

    df_max = omp_df.query("(performance == performance.max())")[["block_size", "performance"]].reset_index(drop=True)
    fig = sns.scatterplot(ax=fig, data=df_max, x='block_size', y='performance', color='red')
    fig.text(x=df_max['block_size'] + 0.2, y=df_max['performance'], s=f"Optimal Blocksize: {df_max.at[0, 'block_size']}")
    fig.set_title("blk_omp, 2048x2048 matrices")
    print(df_max.at[0, 'block_size'])
    plt.savefig('test_img.png')

# def 

if __name__ == '__main__':

    plot_block_experiment("blk_size_2048_20230120_130440")