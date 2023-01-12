import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
from help_funcs import get_plot_folder

# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16

# get data
def get_data(): # TODO update
    Ns = range(10, 100, 10)
    tolerances = [0.0001, 0.00001, 0.000001, 0.0000001]
    err_N_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    err_N_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    err_tol_jacobi = np.random.gamma(1, 0.1, size = len(tolerances))
    err_tol_gauss_seidel = np.random.gamma(1, 0.1, size = len(tolerances))
    return Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel

def plot_error(Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel, plot_folder):
    # make plot
    fig, axes = plt.subplots(1,1, figsize=(10, 10))

    ax = axes
    ax.set_ylabel("Error")
    ax.set_title("UNFINISHED PLOT") # TODO: fix plot when results have been obtained

    # error vs N
    ax.plot(Ns, err_N_jacobi, marker = 'x', color = "C0", label = "Jacobi error vs N")
    ax.plot(Ns, err_N_gauss_seidel, marker = 'o', color = "C1", label = "Gauss-Seidel error vs N")
    ax.set_xlabel("N")

    # error vs tolerance
    ax2 = ax.twiny()
    ax2.plot(tolerances, err_tol_jacobi, marker = 'v', linestyle="--", color = "C0", label = "Jacobi error vs tolerances")
    ax2.plot(tolerances, err_tol_gauss_seidel, marker = '^', linestyle="--", color = "C1", label = "Gauss-Seidel error vs tolerances")
    ax2.set_xlabel("Tolerance")


    # legend
    h1, l1 = ax.get_legend_handles_labels()
    h2, l2 = ax2.get_legend_handles_labels()
    plt.legend(h1+h2, l1+l2, loc="best", fontsize = 12, fancybox = True, framealpha = 1)

    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'error_convergence.pdf')

if __name__ == "__main__":
    plot_folder = get_plot_folder()
    Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel = get_data()
    plot_error(Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel, plot_folder )