import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

PATH = "results/matmult_test_20230105_095258/output_files/20.txt"
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
    name_translator = {"SIZES": "mat_size"}
    int_vars = ["SIZES"]
    with open(exp + fname) as f:
        lines = f.readlines()
        for line in lines:
            name, line = line.split("=")
            setup_data[name_translator[name]] = [int(l.strip()) if name in int_vars else l.strip() for l in line.split(" ") if l not in remove]

    return setup_data


if __name__ == "__main__":
    sns.set_theme()
    exp = "results/blk_test_20230105_112546/"
    df = get_data(exp, 'output_files/30.txt')
    df = df.groupby('blocksize', as_index=False).mean()
    setup_data = get_setup(exp)
    size_data = print(get_sizes(exp))

    print(size_data)

    g = sns.lineplot(df, x="blocksize", y="performance")
    # g.set_xticks([16 + i*16 for i in range(16)])
    g.set_xlabel("Block Size")
    g.set_ylabel("Performance (MBit/s)")
    g.set_title("")

    plt.savefig("Blcksze.png")

