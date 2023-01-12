import numpy as np
import os
import argparse
import re
import pandas as pd

def read_binary(filename):
    x = np.fromfile(filename, dtype=float)
    N = np.cbrt(x.shape[0])
    x = x.reshape(int(N), int(N), int(N))

    return x

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--save_folder", type = str, default="../results")
    parser.add_argument("--output_files", type = str, default="../results")
    args = parser.parse_args()
    return args

def get_plot_folder(args):
    # argparser

    # exp = args.folder + "/" + args.exp + "/"
    plot_folder = args.save_folder + "/plots/"

    # make folder for plots
    os.makedirs(plot_folder, exist_ok = True)

    return plot_folder

def get_dataframe(args): # TODO update
    folder = args.output_files 
    files = []
    max_iter = []
    Ns = []
    iterations = []
    start_t = []
    stepsize = []
    time = []
    diffs = []
    tolerances = []
    error = []
    for file in os.listdir(folder):
        #print(os.path.join(folder, file))
        with open(os.path.join(folder, file), 'r') as f:
            files.append(file)
            data = re.findall(r"[-+]?(?:\d*\.*\d+)", f.read())
            if len(data) == 0:
                continue
            Ns.append(data[0])
            max_iter.append(data[1])
            tolerances.append(data[2])
            start_t.append(data[3])
            stepsize.append(data[4])
            iterations.append(data[5])
            diffs.append(data[6])
            time.append(data[7])
            error.append(data[8])
    
    df = pd.DataFrame({'file': files,
                        'max_iter': max_iter,
                        'N': Ns,
                        'tolerance': tolerances,
                        'time': time,
                        'iterations': iterations,
                        'diffs': diffs,
                        'error': error})
    df = df.astype({'N':'int',
                    'tolerance':'float',
                    'iterations':'int',
                    'diffs':'float',
                    'error':'float',
                    'time':'float'})
    #print(df)
    # make folder for plots
    output_folder = args.save_folder + "/df/"
    os.makedirs(output_folder, exist_ok = True)
    df.to_csv(output_folder + "results.csv")
    return df