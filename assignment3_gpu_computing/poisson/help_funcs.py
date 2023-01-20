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
    # exp = args.folder + "/" + args.exp + "/"
    plot_folder = args.save_folder + "/plots/"

    # make folder for plots
    os.makedirs(plot_folder, exist_ok = True)

    return plot_folder
    
numbers = re.compile(r'(\d+)')
def numericalSort(value):
    parts = numbers.split(value)
    parts[1::2] = map(int, parts[1::2])
    return parts

def get_dataframe(args, subfolder=None): # TODO update
    folder = args.output_files 
    if subfolder is not None:
        folder += "/" + subfolder
    files = []
    max_iter = []
    Ns = []
    iterations = []
    start_t = []
    stepsize = []
    time = []
    timewodt = []
    tolerances = []
    performancewodt = []
    performance = []
    for file in sorted(os.listdir(folder), key=numericalSort):
        #print(os.path.join(folder, file))
        with open(os.path.join(folder, file), 'r') as f:
            files.append(file)
            data = re.findall(r"[-+]?(?:\d*\.*\d+)", f.read())
            #print(data)
            if len(data) == 0:
                continue
            if len(data) == 8:
                Ns.append(data[0])
                max_iter.append(data[1])
                tolerances.append(data[2])
                start_t.append(data[3])
                stepsize.append(data[4])
                iterations.append(data[5])
                timewodt.append(data[6])
                performancewodt.append(np.nan)
                time.append(np.nan)
                performance.append(data[7])
            elif len(data) == 10:
                Ns.append(data[0])
                max_iter.append(data[1])
                tolerances.append(data[2])
                start_t.append(data[3])
                stepsize.append(data[4])
                iterations.append(data[5])
                timewodt.append(data[6])
                performancewodt.append(data[7])
                time.append(data[8])
                performance.append(data[9])
            else:
                Ns.append(data[0])
                max_iter.append(data[1])
                tolerances.append(data[2])
                start_t.append(data[3])
                stepsize.append(data[4])
                if data[5] != 100:
                    iterations.append(data[5])
                    time.append(data[7])
                else:
                    iterations.append(data[6])
                    time.append(np.nan)
                timewodt.append(data[7])
                performance.append(data[8])
                performancewodt.append(np.nan)
    #print(iterations)
    
    df = pd.DataFrame({'file': files,
                        'max_iter': max_iter,
                        'N': Ns,
                        'tolerance': tolerances,
                        'time': time,
                        'iterations': iterations,
                        'Time w/o dt': timewodt,
                        'Performance w/o dt': performancewodt,
                        'Performance': performance})
    df = df.astype({'N':'int',
                    'tolerance':'float',
                    'iterations':'int',
                    'time':'float',
                    'Time w/o dt':'float',
                    'Performance w/o dt':'float',
                    'Performance':'float'})
    #print(df)
    # make folder for plots
    #df.sort_values(by='file', inplace=True)
    df.sort_index()

    output_folder = args.save_folder + "/df/"
    os.makedirs(output_folder, exist_ok = True)
    df.to_csv(output_folder + "results.csv")
    return df