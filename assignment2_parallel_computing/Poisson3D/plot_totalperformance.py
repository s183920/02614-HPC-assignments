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
    threads_n_jacobi_v1_128 = df[df['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v1)')]
    threads_n_jacobi_v1_256 = df[df['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v1)')]
    threads_n_gauss_seidel_v1_128 = df[df['file'].str.contains(r'^(?=.*_gs_N_128)(?=.*v1)')]
    threads_n_gauss_seidel_v1_256 = df[df['file'].str.contains(r'^(?=.*_gs_N_256)(?=.*v1)')]
    threads_n_jacobi_v2_128 = df[df['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v2)')]
    threads_n_jacobi_v2_256 = df[df['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v2)')]
    #speedups
    threads_n_jacobi_v1_128["performance"] = threads_n_jacobi_v1_128.N**3/threads_n_jacobi_v1_128.time/1000*threads_n_jacobi_v1_128.iterations
    threads_n_jacobi_v1_256["performance"] = threads_n_jacobi_v1_256.N**3/threads_n_jacobi_v1_256.time/1000*threads_n_jacobi_v1_256.iterations
    threads_n_gauss_seidel_v1_128["performance"] = threads_n_gauss_seidel_v1_128.N**3/threads_n_gauss_seidel_v1_128.time/1000*threads_n_gauss_seidel_v1_128.iterations
    threads_n_gauss_seidel_v1_256["performance"] = threads_n_gauss_seidel_v1_256.N**3/threads_n_gauss_seidel_v1_256.time/1000*threads_n_gauss_seidel_v1_256.iterations
    threads_n_jacobi_v2_128["performance"] = threads_n_jacobi_v2_128.N**3/threads_n_jacobi_v2_128.time/1000*threads_n_jacobi_v2_128.iterations
    threads_n_jacobi_v2_256["performance"] = threads_n_jacobi_v2_256.N**3/threads_n_jacobi_v2_256.time/1000*threads_n_jacobi_v2_256.iterations

    #plot speedup
    ax.plot(threads, threads_n_jacobi_v1_128.performance, marker = "x", color = "C0", label = "Jacobi_v1_N=128")
    ax.plot(threads, threads_n_jacobi_v1_256.performance, marker = "o", color = "C1", label = "Jacobi_v1_N=256")
    ax.plot(threads, threads_n_gauss_seidel_v1_128.performance, marker = "^", color = "C2", label = "Gauss-Seidel_v1_N=128")
    ax.plot(threads, threads_n_gauss_seidel_v1_256.performance, marker = "v", color = "C3", label = "Gauss-Seidel_v1_N=256")
    ax.plot(threads, threads_n_jacobi_v2_128.performance, marker = "*", color = "C4", label = "Jacobi_v2_N=128")
    ax.plot(threads, threads_n_jacobi_v2_256.performance, marker = "s", color = "C5", label = "Jacobi_v2_N=256")

    #plot linear speedup
    #ax.plot(threads, threads, marker = "P", color = "C6", label = "Linear speedup")
    #ax.plot(threads_n_gauss_seidel_sim.threads, threads_n_gauss_seidel_sim.time, marker = "v", color = "C3", label = "Gauss-Seidel_simple")
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    plt.savefig(plot_folder + 'performance.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_threads(df, plot_folder)