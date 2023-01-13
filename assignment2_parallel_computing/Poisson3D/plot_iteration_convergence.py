import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()
from help_funcs import get_plot_folder, get_args, get_dataframe

# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16

# get data
def get_data(): # TODO update
    Ns = range(10, 100, 10)
    iterations_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    iterations_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    times_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    times_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    return Ns, iterations_jacobi, iterations_gauss_seidel, times_jacobi, times_gauss_seidel

def plot_error(df, plot_folder):
    # make plot
    fig, axes = plt.subplots(1,1, figsize=(15, 10))

    ax = axes
    ax.set_ylabel("Iterations per second")
    ax.set_xlabel("N")
    ax.set_title("Number of iterations before convergence over N for different Thresholds") # TODO: fix plot when results have been obtained
    
    err_n_jacobi = df[df['file'].str.contains("_j_N")].sort_values(by=['N'])
    err_n_gauss_seidel = df[df['file'].str.contains("_gs_N")].sort_values(by=['N'])
    ax.plot(err_n_jacobi.N, err_n_jacobi.iterations, marker = "x", color = "C0", label = "Jacobi iterations vs N, tol = 0.001")
    ax.plot(err_n_gauss_seidel.N, err_n_gauss_seidel.iterations, marker = "o", color = "C1", label = "Gauss-Seidel iterations vs N, tol = 0.001")
    ax.legend(loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'iteration.png')

if __name__ == "__main__":
    args = get_args()
    plot_folder = get_plot_folder(args)
    df = get_dataframe(args)
    plot_error(df, plot_folder)