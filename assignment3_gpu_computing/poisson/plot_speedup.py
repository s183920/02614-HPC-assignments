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
    fig, axes = plt.subplots(1,1, figsize=(10, 5))
    print(df)
    ax = axes
    ax.set_ylabel("Performance [MFLOPS]")
    ax.set_xlabel("Matrix size [N]")
    ax.set_title("Performance vs matrix size")

    jacobi_v0 = df[df['file'].str.contains(rf'^(?=.*v0)')]
    jacobi_v1 = df[df['file'].str.contains(rf'^(?=.*v1)')]
    #jacobi_v2 = df[df['file'].str.contains(rf'^(?=.*v2)')]
    #jacobi_v3 = df[df['file'].str.contains(rf'^(?=.*v3)')]
    jacobi_v4 = df[df['file'].str.contains(rf'^(?=.*v4)')]
    #performance
    #jacobi_v1["performance"] = jacobi_v1["iterations"]*jacobi_v1["N"]**3/jacobi_v1["time"]
    #jacobi_v2["performance"] = jacobi_v2["iterations"]*jacobi_v2["N"]**3/jacobi_v2["time"]
    #jacobi_v3["performance"] = jacobi_v3["iterations"]*jacobi_v3["N"]**3/jacobi_v3["time"]
    #jacobi_v4["performance"] = jacobi_v4["iterations"]*jacobi_v4["N"]**3/jacobi_v4["time"]

    ax.plot(jacobi_v0.N, jacobi_v0["Performance"], marker = "o", color = "C0", label = "GPU with Map and w/o norm calc")
    ax.plot(jacobi_v1.N, jacobi_v1["Performance"], marker = "s", color = "C2", label = "CPU with norm calc")
    #ax.plot(jacobi_v2.N, jacobi_v2["Performance"], marker = "o", color = "C0", label = "1 GPU")
    #ax.plot(jacobi_v3.N, jacobi_v3["Performance"], marker = "x", color = "C1", label = "2 GPU")
    ax.plot(jacobi_v4.N, jacobi_v4["Performance"], marker = "x", color = "C1", label = "GPU with Map and norm calc")

    #ax.plot(jacobi_v1.max_iter, jacobi_v1["Performance"], marker = "s", color = "C2", label = "CPU")
    #ax.plot(jacobi_v2.max_iter, jacobi_v2["Performance"], marker = "o", color = "C0", label = "1 GPU")
    #ax.plot(jacobi_v3.max_iter, jacobi_v3["Performance"], marker = "x", color = "C1", label = "2 GPU")


    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    plt.savefig(plot_folder + 'performanceN.png')


if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_threads(df, plot_folder)
