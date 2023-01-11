import numpy as np
import os
import argparse

def read_binary(filename):
    x = np.fromfile(filename, dtype=float)
    N = np.cbrt(x.shape[0])
    x = x.reshape(int(N), int(N), int(N))

    return x


def get_plot_folder():
    # argparser
    parser = argparse.ArgumentParser()
    parser.add_argument("--save_folder", type = str, default="../results")
    args = parser.parse_args()

    # exp = args.folder + "/" + args.exp + "/"
    plot_folder = args.save_folder + "/plots/"

    # make folder for plots
    os.makedirs(plot_folder, exist_ok = True)

    return plot_folder