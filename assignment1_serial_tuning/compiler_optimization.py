import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()

from help_funcs import get_data, get_sizes, get_setup, get_compiler_optimizations, remove

# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16

if __name__ == "__main__":
    import os
    import argparse

    def_exp = "caches_20230105_143205"
    def_folder = "results"

    # argparser
    parser = argparse.ArgumentParser()
    parser.add_argument("--exp", type = str, default = def_exp)
    parser.add_argument("--folder", type = str, default = def_folder)
    args = parser.parse_args()
    exp = args.folder + "/" + args.exp + "/"
    
    # get setup data
    size_data = pd.DataFrame(get_sizes(exp))
    setup_data = get_setup(exp)

    # get output files
    output_files = [f for f in os.listdir(exp + "output_files") if f.endswith(".txt")]

    # create data
    data = pd.DataFrame()
    for i, f in enumerate(output_files):
        name = f.split(".")[0]
        perm, opt = name.split("_")
        dat = size_data.copy()
        dat = dat.join(get_data(exp, "output_files/" + f))
        dat["permutation"] = perm
        # print(get_compiler_optimizations(exp, f"compile_{opt}.log"))
        dat["optimization"] = [get_compiler_optimizations(exp, f"compile_{opt}.log") for _ in range(len(dat))]

        data = pd.concat((data, dat))
    data.reset_index(inplace = True, drop = True)
    data.to_csv(exp + "data.csv", index = False)

    # get optimization levels
    opts = {opt:get_compiler_optimizations(exp, f"compile_{opt}.log") for opt in range(len(np.unique(data["optimization"])))}

    # plot data
    many_plots = len(data["permutation"].unique()) == 6
    if many_plots:
        fig, axes = plt.subplots(2, 3, figsize = (20, 7.5), sharey=True)
    else:
        fig, axes = plt.subplots(figsize = (20, 7.5), sharey=True)

    # make folder for plots
    os.makedirs(exp + "plots", exist_ok = True)
    
    # loop over permutations
    width = 0.2
    # width = 2 / (len(opts) * 5)
    for i, perm in enumerate(data["permutation"].unique()):
        ax = axes[i//3, i%3] if many_plots else axes
        ax.set_title(f"Permutation: {perm}")
        ax.set_xlabel("Optimizer configuration")
        ax.set_ylabel("Performance (Mflops/s)")
        for j, opt in opts.items():
            # print(perm, opt)
            # print(data[(data["permutation"] == perm)])
            # print(data[data["optimization"].astype(str) == str(opt)])
            plot_data= data[(data["optimization"].astype(str) == str(opt)) & (data["permutation"] == perm)]["performance"]
            ax.bar(j-1.5*width, plot_data.iloc[0], color = "C0", width = width, label = f"L1 cache" if j == 0 else None)
            ax.bar(j-0.5*width, plot_data.iloc[1], color = "C1", width = width, label = f"L2 cache" if j == 0 else None)
            ax.bar(j+0.5*width, plot_data.iloc[2], color = "C2", width = width, label = f"L3 cache" if j == 0 else None)
            ax.bar(j+1.5*width, plot_data.iloc[3], color = "C3", width = width, label = f"main memory" if j == 0 else None)
        ax.set_xticks(range(len(opts)))
        ax.set_xticklabels([f"C{j}" for j in opts.keys()])

    # add legend
    if many_plots:
        axes[0, 1].legend(title = "Color explanation", loc = "upper center", ncol = 4, bbox_to_anchor = (0.5, 1.5))
    else:
        axes.legend(title = "Color explanation", loc = "upper center", ncol = 4, bbox_to_anchor = (0.5, 1.5))
    # axes[0, 2].legend(title = "Color explanation", bbox_to_anchor = (1.1, 0.9),  borderaxespad=0., loc = "upper left", ncol = 1)
    # these are matplotlib.patch.Patch properties
    # props = dict(boxstyle='round', facecolor='lightgrey', alpha=0.5)
    # text_str = "\n".join(["Compiler optimizations"] + ["C" + str(i)+ ": " + str(opt) for i, opt in opts.items()])
    # axes[0, 2].text(1.1, 1, text_str, transform=ax.transAxes, fontsize=14,
    #         verticalalignment='top', bbox=props)
    # axes[1, 0].text(.1, -0.5, text_str, transform=ax.transAxes, fontsize=14,
    #         verticalalignment='top', bbox=props)
        

    # save plot
    plt.tight_layout()
    plt.savefig(exp + "plots/optimizers.pdf")


