from pathlib import Path
import pandas as pd

def read_experiment(experiment_name: str):
    output = Path("results") / experiment_name / "output_files"
    dfs = []
    for file in output.glob("*.txt"):
        dfs.append(read_file(file))
    
    return pd.concat(dfs).reset_index()
    
def read_file(file: Path) -> pd.DataFrame:
    columns = []
    values = []
    remove = ["", " ", "#"]
    with file.open() as f:
        for line in f:
            key, value = line.split(": ")
            if key.lower() in "results":
                value = value.split(" ")
                values += [l.strip() for l in value if l not in remove]
                if values[-2] == "matmult_blk":
                    del values[-1]
                columns += ["memory", "performance", "result", "info"]

            else:
                columns += [key]
                values += [value.strip()]
    df = pd.DataFrame(data=[values], columns=columns)
    return df
            
if __name__ == "__main__":
    df = read_experiment("blk_size_500_20230118_134540")
    print(df)