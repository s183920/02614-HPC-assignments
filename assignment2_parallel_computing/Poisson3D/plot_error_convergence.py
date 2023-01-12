import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re
sns.set()
sns.set_style("whitegrid")
from help_funcs import get_plot_folder

# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16

# get data
def get_data(test_folder): # TODO update
    folder = test_folder + "/exp_error_convergence/output_files"
    files = []
    max_iter = []
    Ns = []
    iterations = []
    time = []
    diffs = []
    tolerances = []
    error = []
    for file in os.listdir(folder):
        print(os.path.join(folder, file))
        with open(os.path.join(folder, file), 'r') as f:
            files.append(file)
            data = re.findall(r"[-+]?(?:\d*\.*\d+)", f.read())
            max_iter.append(data[0])
            Ns.append(data[1])
            tolerances.append(data[2])
            time.append(data[3])
            iterations.append(data[4])
            diffs.append(data[5])
            error.append(data[6])
    
    df = pd.DataFrame({'file': files,
                        'max_iter': max_iter,
                        'N': Ns,
                        'tolerances': tolerances,
                        'time': time,
                        'iterations': iterations,
                        'diffs': diffs,
                        'error': error})
    print(df)
            
    

    #Ns = np.linspace(10, 200, 20)
    #tolerances = [0.0001, 0.00001, 0.000001, 0.0000001]
    #tolerances = np.linspace(0.1, 0.0000001, 20)
    #err_N_jacobi = np.random.gamma(1, 0.1, size = len(Ns))
    #err_N_gauss_seidel = np.random.gamma(1, 0.1, size = len(Ns))
    #err_tol_jacobi = np.random.gamma(1, 0.1, size = len(tolerances))
    #err_tol_gauss_seidel = np.random.gamma(1, 0.1, size = len(tolerances))
    #return Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel

def plot_error(Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel, plot_folder):
    # make plot
    fig, axes = plt.subplots(2,1, figsize=(10, 10))

    ax = axes
    ax[0].set_ylabel("Error")
    ax[0].set_title("UNFINISHED PLOT") # TODO: fix plot when results have been obtained

    # error vs N
    ax[0].plot(Ns, err_N_jacobi, marker = 'x', color = "C0", label = "Jacobi error vs N")
    ax[0].plot(Ns, err_N_gauss_seidel, marker = 'o', color = "C1", label = "Gauss-Seidel error vs N")
    ax[0].set_xlabel("N")
    # error vs tolerance
    #ax2 = ax.twiny()
    ax[1].set_ylabel("Error")
    ax[1].plot(tolerances, err_tol_jacobi, marker = 'v', linestyle="--", color = "C0", label = "Jacobi error vs tolerances")
    ax[1].plot(tolerances, err_tol_gauss_seidel, marker = '^', linestyle="--", color = "C1", label = "Gauss-Seidel error vs tolerances")
    ax[1].set_xlabel("Tolerance")


    # legend
    h1, l1 = ax[0].get_legend_handles_labels()
    h2, l2 = ax[1].get_legend_handles_labels()
    plt.legend(h1+h2, l1+l2, loc="best", fontsize = 12, fancybox = True, framealpha = 1)
    sns.move_legend(ax[1], "upper left", bbox_to_anchor=(1, 1))
    fig.tight_layout()

    # save plot
    plt.savefig(plot_folder + 'error_convergence.pdf')

if __name__ == "__main__":
    #plot_folder = get_plot_folder()
    #Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel = get_data()
    #plot_error(Ns, tolerances, err_N_jacobi, err_N_gauss_seidel, err_tol_jacobi, err_tol_gauss_seidel, plot_folder )
    get_data("../results/test_20230112_112226")