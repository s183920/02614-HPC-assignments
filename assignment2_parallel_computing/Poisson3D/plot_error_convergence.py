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

# get data
def get_data(): # TODO update
    #Ns = np.linspace(10, 200, 20)
    #tolerances = [0.0001, 0.00001, 0.000001, 0.0000001]
    #tolerances = np.linspace(0.1, 0.0000001, 20)
    #err_N_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    #err_N_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    #err_tol_jacobi = np.random.gamma(1, 0.1, size = len(tolerances))
    #err_tol_gauss_seidel = np.random.gamma(1, 0.1, size = len(tolerances))
    #return Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel
    return None, None, None, None, None, None

def plot_error(df, plot_folder):
    # make plot
    err_n_jacobi = df[(df['tolerance'] == 0.01) & (df['file'].str.contains("_j_N"))].sort_values(by=['N'])
    err_n_gauss_seidel = df[(df['tolerance'] == 0.01) & (df['file'].str.contains("_gs_N"))].sort_values(by=['N'])
    err_tol_jacobi = df[(df['N'] == 100) & (df['file'].str.contains("_j_tol"))].sort_values(by=['tolerance'])
    err_tol_gauss_seidel = df[(df['N'] == 100) & (df['file'].str.contains("_gs_tol"))].sort_values(by=['tolerance'])
    fig, axes = plt.subplots(2,1, figsize=(15, 10))

    ax = axes
    ax[0].set_ylabel("Error")
    ax[0].set_title("N vs Error") # TODO: fix plot when results have been obtained

    # error vs N
    ax[0].plot(err_n_jacobi.N, err_n_jacobi.error, marker = 'x', color = "C0", label = "Jacobi error vs N")
    ax[0].plot(err_n_gauss_seidel.N, err_n_gauss_seidel.error, marker = 'o', color = "C1", label = "Gauss-Seidel error vs N")
    ax[0].set_xlabel("N")
    ax[0].legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    # error vs tolerance
    #ax2 = ax.twiny()
    ax[1].set_title("Tolerance vs Error")
    ax[1].set_ylabel("Error")
    ax[1].plot(err_tol_jacobi.tolerance, err_tol_jacobi.error, marker = 'x', linestyle="-", color = "C0", label = "Jacobi error vs tolerances")
    ax[1].plot(err_tol_gauss_seidel.tolerance, err_tol_gauss_seidel.error, marker = 'o', linestyle="-", color = "C1", label = "Gauss-Seidel error vs tolerances")
    ax[1].set_xlabel("Tolerance")
    ax[1].semilogx()
    ax[1].legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)

    # legend
    #h1, l1 = ax[0].get_legend_handles_labels()
    #h2, l2 = ax[1].get_legend_handles_labels()
    #plt.legend(h1+h2, l1+l2, loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    #sns.move_legend(ax[1], "upper left", bbox_to_anchor=(1, 1))
    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'error_convergence.png')

    fig, axes = plt.subplots(1,1, figsize=(15, 10))

    ax = axes
    ax.set_ylabel("Iterations per second")
    ax.set_xlabel("N")
    ax.set_title("Number of iterations before convergence over N for different Thresholds") # TODO: fix plot when results have been obtained
    testtols = [0.01, 0.001, 0.0001]
    colors = ["C0", "C1", "C2", "C3", "C4", "C5"]
    markers = ["x", "o", "s", "d", "v", "^"]
    for i in range(len(testtols)):
        err_n_jacobi = df[(df['tolerance'] == testtols[i]) & (df['file'].str.contains("_j_N"))].sort_values(by=['N'])
        err_n_gauss_seidel = df[(df['tolerance'] == testtols[i]) & (df['file'].str.contains("_gs_N"))].sort_values(by=['N'])
        ax.plot(err_n_jacobi.N, err_n_jacobi.iterations, marker = markers[i], color = colors[i], label = "Jacobi iterations vs N, tol = " + str(testtols[i]))
        ax.plot(err_n_gauss_seidel.N, err_n_gauss_seidel.iterations, marker = markers[i+3], linestyle="--", color = colors[i+3], label = "Gauss-Seidel iterations vs N, tol = " + str(testtols[i]))
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'iteration.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    #Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel = get_data()
    plot_error(df, plot_folder)