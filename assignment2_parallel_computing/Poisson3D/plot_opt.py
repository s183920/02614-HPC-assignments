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

def plot_opt(df, plot_folder):
    # make plot
    df_opt = get_dataframe(args, subfolder="with_optimization")
    df_no_opt = get_dataframe(args, subfolder="without_optimization")

    fig, axes = plt.subplots(2,1, figsize=(15, 10), sharey=True)
    #print(df)
    ax = axes
    ax[0].set_ylabel("Speedup")
    ax[0].set_xlabel("Number of Threads")
    ax[1].set_xlabel("Number of Threads")
    fig.suptitle("Speedup over number of Threads") # TODO: fix plot when results have been obtained

    
    threads = list(range(1,21))
    threads_n_jacobi_v1_128_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v1)')]
    threads_n_jacobi_v1_256_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v1)')]
    threads_n_gauss_seidel_v1_128_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_gs_N_128)(?=.*v1)')]
    threads_n_gauss_seidel_v1_256_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_gs_N_256)(?=.*v1)')]
    threads_n_jacobi_v2_128_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v2)')]
    threads_n_jacobi_v2_256_opt = df_opt[df_opt['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v2)')]

    threads_n_jacobi_v1_128_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v1)')]
    threads_n_jacobi_v1_256_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v1)')]
    threads_n_gauss_seidel_v1_128_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_gs_N_128)(?=.*v1)')]
    threads_n_gauss_seidel_v1_256_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_gs_N_256)(?=.*v1)')]
    threads_n_jacobi_v2_128_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_j_N_128)(?=.*v2)')]
    threads_n_jacobi_v2_256_no_opt = df_no_opt[df_no_opt['file'].str.contains(r'^(?=.*_j_N_256)(?=.*v2)')]

    print(threads_n_jacobi_v1_128_opt)
    #speedups
    threads_n_jacobi_v1_128_opt.speedup = threads_n_jacobi_v1_128_opt.time[0]/threads_n_jacobi_v1_128_opt.time
    threads_n_jacobi_v1_256_opt.speedup = threads_n_jacobi_v1_256_opt.time[0]/threads_n_jacobi_v1_256_opt.time
    threads_n_gauss_seidel_v1_128_opt.speedup = threads_n_gauss_seidel_v1_128_opt.time[0]/threads_n_gauss_seidel_v1_128_opt.time
    threads_n_gauss_seidel_v1_256_opt.speedup = threads_n_gauss_seidel_v1_256_opt.time[0]/threads_n_gauss_seidel_v1_256_opt.time
    threads_n_jacobi_v2_128_opt.speedup = threads_n_jacobi_v2_128_opt.time[0]/threads_n_jacobi_v2_128_opt.time
    threads_n_jacobi_v2_256_opt.speedup = threads_n_jacobi_v2_256_opt.time[0]/threads_n_jacobi_v2_256_opt.time

    threads_n_jacobi_v1_128_no_opt.speedup = threads_n_jacobi_v1_128_no_opt.time[0]/threads_n_jacobi_v1_128_no_opt.time
    threads_n_jacobi_v1_256_no_opt.speedup = threads_n_jacobi_v1_256_no_opt.time[0]/threads_n_jacobi_v1_256_no_opt.time
    threads_n_gauss_seidel_v1_128_no_opt.speedup = threads_n_gauss_seidel_v1_128_no_opt.time[0]/threads_n_gauss_seidel_v1_128_no_opt.time
    threads_n_gauss_seidel_v1_256_no_opt.speedup = threads_n_gauss_seidel_v1_256_no_opt.time[0]/threads_n_gauss_seidel_v1_256_no_opt.time
    threads_n_jacobi_v2_128_no_opt.speedup = threads_n_jacobi_v2_128_no_opt.time[0]/threads_n_jacobi_v2_128_no_opt.time
    threads_n_jacobi_v2_256_no_opt.speedup = threads_n_jacobi_v2_256_no_opt.time[0]/threads_n_jacobi_v2_256_no_opt.time
    
    #plot speedup
    ax[0].plot(threads, threads_n_jacobi_v1_128_opt.speedup, marker = "x", color = "C0", label = "Jacobi_v1_opt_N=128")
    ax[0].plot(threads, threads_n_jacobi_v1_256_opt.speedup, marker = "o", color = "C1", label = "Jacobi_v1_opt_N=256")
    ax[0].plot(threads, threads_n_gauss_seidel_v1_128_opt.speedup, marker = "v", color = "C2", label = "Gauss-Seidel_v1_opt_N=128")
    ax[0].plot(threads, threads_n_gauss_seidel_v1_256_opt.speedup, marker = "s", color = "C3", label = "Gauss-Seidel_v1_opt_N=256")
    ax[0].plot(threads, threads_n_jacobi_v2_128_opt.speedup, marker = "^", color = "C4", label = "Jacobi_v2_opt_N=128")
    ax[0].plot(threads, threads_n_jacobi_v2_256_opt.speedup, marker = "D", color = "C5", label = "Jacobi_v2_opt_N=256")
    ax[1].plot(threads, threads_n_jacobi_v1_128_no_opt.speedup, marker = "x", color = "C0", label = "Jacobi_v1_no_opt_N=128")
    ax[1].plot(threads, threads_n_jacobi_v1_256_no_opt.speedup, marker = "o", color = "C1", label = "Jacobi_v1_no_opt_N=256")
    ax[1].plot(threads, threads_n_gauss_seidel_v1_128_no_opt.speedup, marker = "v", color = "C2", label = "Gauss-Seidel_v1_no_opt_N=128")
    ax[1].plot(threads, threads_n_gauss_seidel_v1_256_no_opt.speedup, marker = "s", color = "C3", label = "Gauss-Seidel_v1_no_opt_N=256")
    ax[1].plot(threads, threads_n_jacobi_v2_128_no_opt.speedup, marker = "^", color = "C4", label = "Jacobi_v2_no_opt_N=128")
    ax[1].plot(threads, threads_n_jacobi_v2_256_no_opt.speedup, marker = "D", color = "C5", label = "Jacobi_v2_no_opt_N=256")


    #plot linear speedup
    ax[0].plot(threads, threads, marker = "*", color = "C6", label = "Linear speedup")
    ax[1].plot(threads, threads, marker = "*", color = "C6", label = "Linear speedup")
    #ax.plot(threads_n_gauss_seidel_sim.threads, threads_n_gauss_seidel_sim.time, marker = "v", color = "C3", label = "Gauss-Seidel_simple")
    ax[0].legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    ax[1].legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    plt.savefig(plot_folder + 'opt.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    plot_opt(args, plot_folder)

