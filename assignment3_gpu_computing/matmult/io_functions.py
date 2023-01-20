from pathlib import Path
import pandas as pd

def read_experiment(experiment_name: str):
    output = Path("results") / experiment_name / "output_files"
    dfs = []
    for file in output.glob("*.txt"):
        dfs.append(read_file(file))
    
    return pd.concat(dfs).reset_index(drop=True)
    
def read_file(file: Path) -> pd.DataFrame:
    df_dict = {}
    with file.open() as f:
        for line in f:
            key, value = line.split(": ")
            if key.lower() in "results":
                result_loader(value, df_dict)
            elif 'time' in key.lower():
                time_loader(key, value, df_dict)
            else:
                df_dict.update({key: value.strip()})

    df = pd.DataFrame(data=[values], columns=columns)
    return df
            
def result_loader(value, df_dict: dict):
    remove = ["", " ", "#"]
    value = value.split(" ")
    values = [l.strip() for l in value if l not in remove]
    if values[-2] == "matmult_blk":
        del values[-1]
    columns = ["memory", "performance", "result", "info"]
    df_dict.update(dict(zip(columns, values)))


def time_loader(key: str, value: str, df_dict: dict):
    df_dict[key + '_count'] = df.get('time_count', 0) + 1
    df_dict[key] = value.strip()



if __name__ == "__main__":
    df = read_experiment("blk_size_500_20230118_134540")
    print(df)