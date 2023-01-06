import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

PATH = "results/blk_outer_inner_20230106_090532/output_files/500.txt"
remove = ["", " ", "#"]
# plot settings
plt.rcParams["font.size"] = 16
plt.rcParams["axes.labelsize"] = 16
plt.rcParams["axes.titlesize"] = 16


def get_data(exp, fname):
    data = pd.DataFrame(columns = ["memory", "performance", "blocksize", "info"])

    with open(exp + fname) as f:
        lines = f.readlines()
        
        for i, line in enumerate(lines):
            line = line.split(" ")
            line = [l.strip() for l in line if l not in remove]
            data.loc[i, "memory"] = line[0]
            data.loc[i, "performance"] = line[1]
            data.loc[i, "info"] = line[2]
            data.loc[i, "blocksize"] = line[3]
    
    data["memory"] = data["memory"].astype(float)
    data["performance"] = data["performance"].astype(float)
    data = data.astype({
        "memory": float,
        "performance": float,
        "info": str,
        "blocksize": int
    })

    return data

def get_setup(exp, fname = "setup.txt"):
    setup_data = {}
    with open(exp + fname, "r") as f:
        lines = f.readlines()
        for line in lines:
            name, line = line.split(": ")
            try:
                setup_data[name.strip()] = eval(line.strip())
            except:
                setup_data[name.strip()] = line.strip()

    return setup_data

def get_sizes(exp, fname = "setup_sizes.txt"):
    setup_data = {}
    name_translator = {"SIZES": "mat_size", "BLKSIZE": "block_size"}
    int_vars = ["SIZES"]
    with open(exp + fname) as f:
        lines = f.readlines()
        for line in lines:
            name, line = line.split("=")
            setup_data[name_translator[name]] = [int(l.strip()) if name in int_vars else l.strip() for l in line.split(" ") if l not in remove]

    return setup_data


if __name__ == "__main__":
    sns.set_theme()
    exp = "results_saved/blk_size_1200_p2/"
    df = get_data(exp, 'output_files/1200.txt')
    df = df.groupby('blocksize', as_index=False).mean()
    # setup_data = get_setup(exp)
    # size_data = print(get_sizes(exp))

    # print(size_data)

    g = sns.lineplot(df, x="blocksize", y="performance")
    # g.set_xticks([16 + i*16 for i in range(16)])
    g.set_xlabel("Block Size")
    g.set_xscale("log", base=2)
    g.set_ylabel("Performance (MFlops/s)")
    g.set_title("Blocksize for matrix size 1200x1200")
    g.axhline(6541.119, color='k', linestyle='--', label='Baseline')
    g.legend()
    plt.savefig("plot_for_report/Blocksize1200.png")

