import io_functions
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
sns.set_style("whitegrid")
# sns.set(rc={'figure.figsize':(11.7,8.27)})

def plot_block_experiment(experiment: str):
    df = io_functions.read_experiment(experiment)
    print(df)
    # fig = sns.lineplot(data=df, x='block_size', y='performance', hue='version')
    # plt.savefig('test_img.png')

# def 

if __name__ == '__main__':

    plot_block_experiment("blk_size_2048_20230120_110815")