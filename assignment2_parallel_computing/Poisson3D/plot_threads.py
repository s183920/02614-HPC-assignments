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
    ax.set_ylabel("Speedup")
    ax.set_xlabel("Number of Threads")
    ax.set_title("Speedup over number of Threads") # TODO: fix plot when results have been obtained
    
    threads = list(range(1,21))
    threads_n_jacobi_v1_128 = df[df['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v1)')]
    threads_n_jacobi_v1_256 = df[df['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v1)')]
    threads_n_gauss_seidel_v1_128 = df[df['file'].str.contains(r'^(?=.*_gs_N_128)(?=.*v1)')]
    threads_n_gauss_seidel_v1_256 = df[df['file'].str.contains(r'^(?=.*_gs_N_256)(?=.*v1)')]
    threads_n_jacobi_v2_128 = df[df['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v2)')]
    threads_n_jacobi_v2_256 = df[df['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v2)')]
    #speedups
    threads_n_jacobi_v1_128["speedup"] = threads_n_jacobi_v1_128.time.iloc[0]/threads_n_jacobi_v1_128.time
    threads_n_jacobi_v1_256["speedup"] = threads_n_jacobi_v1_256.time.iloc[0]/threads_n_jacobi_v1_256.time
    threads_n_gauss_seidel_v1_128["speedup"] = threads_n_gauss_seidel_v1_128.time.iloc[0]/threads_n_gauss_seidel_v1_128.time
    threads_n_gauss_seidel_v1_256["speedup"] = threads_n_gauss_seidel_v1_256.time.iloc[0]/threads_n_gauss_seidel_v1_256.time
    threads_n_jacobi_v2_128["speedup"] = threads_n_jacobi_v2_128.time.iloc[0]/threads_n_jacobi_v2_128.time
    threads_n_jacobi_v2_256["speedup"] = threads_n_jacobi_v2_256.time.iloc[0]/threads_n_jacobi_v2_256.time

    #plot speedup
    ax.plot(threads, threads_n_jacobi_v1_128.speedup, marker = "x", color = "C0", label = "Jacobi_v1_N=128")
    ax.plot(threads, threads_n_jacobi_v1_256.speedup, marker = "o", color = "C1", label = "Jacobi_v1_N=256")
    ax.plot(threads, threads_n_gauss_seidel_v1_128.speedup, marker = "^", color = "C2", label = "Gauss-Seidel_v1_N=128")
    ax.plot(threads, threads_n_gauss_seidel_v1_256.speedup, marker = "v", color = "C3", label = "Gauss-Seidel_v1_N=256")
    ax.plot(threads, threads_n_jacobi_v2_128.speedup, marker = "*", color = "C4", label = "Jacobi_v2_N=128")
    ax.plot(threads, threads_n_jacobi_v2_256.speedup, marker = "s", color = "C5", label = "Jacobi_v2_N=256")

    #plot linear speedup
    ax.plot(threads, threads, marker = "v", color = "C3", label = "Linear speedup")
    #ax.plot(threads_n_gauss_seidel_sim.threads, threads_n_gauss_seidel_sim.time, marker = "v", color = "C3", label = "Gauss-Seidel_simple")
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    plt.savefig(plot_folder + 'threads.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_threads(df, plot_folder)