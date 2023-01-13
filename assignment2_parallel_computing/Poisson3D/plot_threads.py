import os
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

    ax = axes
    ax.set_ylabel("Time")
    ax.set_xlabel("Number of Threads")
    ax.set_title("Number of iterations before convergence over N") # TODO: fix plot when results have been obtained
    df["threads"] = df["file"].str.extract(r"_(\d+)t_")
    threads_n_jacobi = df[df['file'].str.contains("_j_N")].sort_values(by=['N'])
    threads_n_gauss_seidel = df[df['file'].str.contains("_gs_N")].sort_values(by=['N'])
    ax.plot(threads_n_jacobi.threads, threads_n_jacobi.time, marker = "x", color = "C0", label = "Jacobi")
    ax.plot(threads_n_gauss_seidel.threads, threads_n_gauss_seidel.time, marker = "o", color = "C1", label = "Gauss-Seidel")
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_error(df, plot_folder)