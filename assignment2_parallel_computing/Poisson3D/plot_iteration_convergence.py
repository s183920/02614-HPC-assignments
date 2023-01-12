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
    iterations_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    iterations_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    times_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    times_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    return Ns, iterations_jacobi, iterations_gauss_seidel, times_jacobi, times_gauss_seidel

def plot_error(Ns, iterations_jacobi, iterations_gauss_seidel, times_jacobi, times_gauss_seidel, plot_folder):
    # make plot
    fig, axes = plt.subplots(1,1, figsize=(10, 10))

    ax = axes
    ax.set_ylabel("Iterations per second")
    ax.set_xlabel("N")
    ax.set_title("UNFINISHED PLOT") # TODO: fix plot when results have been obtained
    
    ax.plot(Ns, iterations_jacobi/times_jacobi, label = "Jacobi", color = "C0", marker = "x")
    ax.plot(Ns, iterations_gauss_seidel/times_gauss_seidel, label = "Gauss-Seidel", color = "C1", marker = "o")
    
    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'iteration.pdf')

if __name__ == "__main__":
    plot_folder = get_plot_folder()
    Ns, iterations_jacobi, iterations_gauss_seidel, times_jacobi, times_gauss_seidel = get_data()
    plot_error(Ns, iterations_jacobi, iterations_gauss_seidel, times_jacobi, times_gauss_seidel, plot_folder )