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

    def_exp = "large_20230104_222614"
    def_folder = "results_saved"
    def_compile_count = ""

    # argparser
    parser = argparse.ArgumentParser()
    parser.add_argument("--exp", type = str, default = def_exp)
    parser.add_argument("--folder", type = str, default = def_folder)
    parser.add_argument("--compile_count", type = str, default = def_compile_count)
    args = parser.parse_args()
    exp = args.folder + "/" + args.exp + "/"
    opt = "_" + args.compile_count if args.compile_count != "" else ""
    
    # make folder for plots
    os.makedirs(exp + "plots", exist_ok = True)

    # get setup data
    size_data = pd.DataFrame(get_sizes(exp))
    setup_data = get_setup(exp)


    # calculate flops
    flops = 2*size_data["mat_size"]**3
    
    # get output files
    output_files = [f for f in os.listdir(exp + "output_files") if f.endswith(".txt")]

    # create plots
    n_rows = 1
    n_cols = 2

    fig, axes = plt.subplots(n_rows, n_cols, figsize = (n_cols * 7.5, n_rows * 5))
    fig.suptitle("Performance for different matrix sizes")# \n Compiler optimizations: " + str(" ".join(get_compiler_optimizations(exp, f"compile{opt}.log"))))

    # performance plot
    ax = axes[0]
    ax.set_title("Performance plot")
    ax.set_xlabel("Memory (kB)")
    ax.set_ylabel("Performance (Mflops/s)")
    c1 = int(setup_data["L1d cache"].replace("K", ""))
    c2 = int(setup_data["L2 cache"].replace("K", ""))
    c3 = int(setup_data["L3 cache"].replace("K", ""))
    ax.axvline(c1, linestyle = "--", color = "orange", alpha = .5, label = "L1 cache")
    ax.axvline(c2, linestyle = "--", color = "green", alpha = .5, label = "L2 cache")
    ax.axvline(c3, linestyle = "--", color = "blue", alpha = .5, label = "L3 cache")

    # time vs size plot
    ax = axes[1]
    ax.set_title("Time vs size plot")
    ax.set_xlabel("Matrix size")
    ax.set_ylabel("Time (s)")
    # legend hack
    ax.plot([], [], color = "orange", alpha = 0.5, linestyle = "--", label = "L1 cache")
    ax.plot([], [], color = "green", alpha = 0.5, linestyle = "--", label = "L2 cache")
    ax.plot([], [], color = "blue", alpha = 0.5, linestyle = "--", label = "L3 cache")

    # performance vs size plot
    # ax = axes[2]
    # ax.set_title("Performance vs size plot")
    # ax.set_xlabel("Matrix size")
    # ax.set_ylabel("Performance (Mflops/s)")
  
    for i, f in enumerate(output_files):
        name = f.split(".")[0]
        data = size_data.copy()
        data = data.join(get_data(exp, "output_files/" + f))

        # performance plot
        ax = axes[0]
        ax.plot(data["memory"], data["performance"] ,label = name)
        ax.set_xscale("log", base = 2) 
        # ticks = range(0, 14)
        ticks = [2**i for i in range(0, 16)] #  1, 2, 4, 8, 16, 64, 256, 1024, 4096, 16384, 65536, 262144
        # ax.set_xticks([2**t for t in ticks])
        # ax.set_xticklabels(["$2^{"+ str(t) + "}$" for t in ticks])
        ax.set_xticks(ticks)
        ticks[::2] = [""]*len(ticks[::2])
        ax.set_xticklabels(ticks) 

        # time vs size plot
        ax = axes[1]
        ax.plot(data["mat_size"], flops/(1e6*data["performance"]), label = name)
        # ax.set_yscale("log", base = 2)

        # performance vs size plot
        # ax = axes[2]
        # ax.plot(data["mat_size"], data["performance"], label = name)
        # ax.set_xscale("log", base = 2)
    
    # for ax in axes:
    #     ax.legend(title = "Permutation")
    axes[1].legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.tight_layout()
    plt.savefig(exp + "plots/size_performance.pdf")