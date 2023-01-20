import io_functions
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
sns.set(rc={'figure.figsize':(9,6)})
sns.set_style("whitegrid")
# sns.set(rc={'figure.figsize':(11.7,8.27)})

markers = {
    "mkn_omp" : "o", 
    "lib" : "x", 
    "blk_omp": "^", 
    "linear": "D", 
    "mkn_offload": "s", 
    "blk_offload": "v", 
    "mnk_offload": "p",
    "asy_offload": "P",
}

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

def plot_ex1(exp_name: str):
    df = io_functions.read_experiment(exp_name)
    df = df.astype({'performance': float, 'block_size': int, 'threads': int})
    df["time_avg"] = df.Time / df.Time_count

    fig, axes = plt.subplots(1,2, figsize=(10,5))
    fig.suptitle("Performance of matmul with matrix size 2048")

    for version in df.version.unique():
        df_version = df[df.version == version]
        df_version = df_version.sort_values(by='threads')
        t0 = df_version[df_version.threads==1].time_avg.item()
        axes[0].plot(df_version.threads, df_version.performance, label=version, marker = markers[version])
        axes[1].plot(df_version.threads, t0/df_version.time_avg, label=version, marker = markers[version])
    
    ax = axes[0]
    ax.legend()
    ax.set_xlabel("Threads")
    ax.set_ylabel("Performance (GFlops/s)")

    ax = axes[1]
    ax.set_xlabel("Threads")
    ax.set_ylabel("Speedup")
    ax.plot([1, 16], [1, 16], label="Linear speedup", marker = markers["linear"])
    ax.legend()

    fig.tight_layout()
    plt.savefig(f'results/{exp_name}/plots/ex1_results.pdf')

    plt.show()

def plot_ex2(exp_name: str):
    df = io_functions.read_experiment(exp_name)
    df = df.astype({'performance': float, 'teams': int, 'threads': int})
    df["performance"] = df["performance"] / 1000
    df = df.sort_values(by=['teams'])
    # df["time_avg"] = df["Time with transfer"] / df["Time with transfer_count"]
    # df["transfer_time_avg"] = df["Time without transfer"] / df["Time without transfer_count"] # without 
    # print(df)
    fig, axes = plt.subplots()

    for version in df.version.unique():
        df_version = df[df.version == version]
        axes.plot(df_version.teams, df_version.performance, label=version, marker = markers[version])
    axes.legend()
    axes.set_xlabel("Teams")
    axes.set_ylabel("Performance (GFlops/s)")

    fig.tight_layout()
    plt.savefig(f'results/{exp_name}/plots/ex2_results.pdf')

    plt.show()

def plot_ex4(exp_name: str):
    df = io_functions.read_experiment(exp_name)
    df = df.astype({'performance': float, 'teams': int, 'threads': int, 'slabs': float})
    print(df)
    df["performance"] = df["performance"] / 1000
    df = df.sort_values(by=['slabs'])
    df["time_avg"] = df["Time with transfer"] / df["Time with transfer_count"]
    # df["transfer_time_avg"] = df["Time without transfer"] / df["Time without transfer_count"] # without 
    # print(df)
    fig, axes = plt.subplots()

    df_version = df[df.version == "asy_offload"]
    axes.plot(df_version.slabs, df_version.time_avg, label="asy_offload", marker = markers["asy_offload"])
    axes.axhline(y = df[df.version == "blk_offload"].time_avg.item(), linestyle = "--", color = "C3", label="blk_offload")
    axes.legend()
    axes.set_xlabel("Slabs")
    axes.set_ylabel("Time (ms)")

    fig.tight_layout()
    plt.savefig(f'results/{exp_name}/plots/ex4_results.pdf')

    plt.show()


if __name__ == '__main__':
    import argparse
    import os
    parser = argparse.ArgumentParser()
    parser.add_argument('-q', type=int, help='Question to plot')
    parser.add_argument('--expname', type=str, help='Name of the experiment to plot')
    # plot_block_experiment("blk_size_500_20230118_134540")

    args = parser.parse_args()

    os.makedirs(f"results/{args.expname}/plots", exist_ok=True)

    if args.q == 0:
        plot_block_experiment(args.expname)

    if args.q == 1:
        plot_ex1(args.expname)

    if args.q == 2:
        plot_ex2(args.expname)

    if args.q == 4:
        plot_ex4(args.expname)

    
