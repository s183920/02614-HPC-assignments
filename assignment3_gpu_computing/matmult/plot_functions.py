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

    if args.q == 2:
        plot_ex2(args.expname)
