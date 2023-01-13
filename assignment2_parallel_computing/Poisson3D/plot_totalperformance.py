import os
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
sns.set_style("whitegrid")
from help_funcs import get_plot_folder, get_dataframe, get_args

# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16

def plot_threads(df, plot_folder):
    # make plot
    fig, axes = plt.subplots(1,1, figsize=(15, 10))
    print(df)
    ax = axes
    ax.set_ylabel("MLups/s")
    ax.set_xlabel("Number of Threads")
    ax.set_title("Total Performance over number of Threads") # TODO: fix plot when results have been obtained
    #df["threads"] = df["file"].str.extract(r"_(\d+)t_")
    #df["threads"] = [int(re.findall(r"[-+]?(?:\d*\.*\d+)", df["file"][i])[1]) for i in range(len(df["file"]))]
    #df.sort_values(by=['threads'], inplace = True)
    threads = list(range(1,21))
    threads_n_jacobi_v1 = df[df['file'].str.contains(r'^(?=.*_j_)(?=.*v1)')]
    threads_n_gauss_seidel_v1 = df[df['file'].str.contains(r'^(?=.*_gs_)(?=.*v1)')]
    threads_n_jacobi_v2 = df[df['file'].str.contains(r'^(?=.*_j_)(?=.*v2)')]
    threads_n_jacobi_v1["performance"] = threads_n_jacobi_v1.N**3/threads_n_jacobi_v1.time/1000*threads_n_jacobi_v1.iterations
    threads_n_gauss_seidel_v1["performance"] = threads_n_gauss_seidel_v1.N**3/threads_n_gauss_seidel_v1.time/1000*threads_n_gauss_seidel_v1.iterations
    threads_n_jacobi_v2["performance"] = threads_n_jacobi_v2.N**3/threads_n_jacobi_v2.time/1000*threads_n_jacobi_v2.iterations
    #print(threads_n_jacobi_v1)

    #threads_n_gauss_seidel_sim = df[df['file'].str.contains(r'^(?=.*_gs_)(?=.*v2)')]
    ax.plot(threads, threads_n_jacobi_v1.performance, marker = "x", color = "C0", label = "Jacobi_v1")
    #ax.plot(threads, threads_n_gauss_seidel_v1.performance, marker = "o", color = "C1", label = "Gauss-Seidel_v2")
    ax.plot(threads, threads_n_jacobi_v2.performance, marker = "^", color = "C2", label = "Jacobi_v2")
    #plot linear speedup
    #ax.plot(threads, threads, marker = "v", color = "C3", label = "Linear speedup")
    #ax.plot(threads_n_gauss_seidel_sim.threads, threads_n_gauss_seidel_sim.time, marker = "v", color = "C3", label = "Gauss-Seidel_simple")
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    plt.savefig(plot_folder + 'performance.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_threads(df, plot_folder)