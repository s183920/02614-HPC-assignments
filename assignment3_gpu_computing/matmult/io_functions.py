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
            lst = line.split(": ")
            if len(lst) < 2:
                result_loader(lst[0], df_dict)
            else: 
                key, value = lst
                if 'time' in key.lower():
                    time_loader(key, value, df_dict)
                else:
                    df_dict.update({key: value.strip()})
    

    df = pd.DataFrame.from_records([df_dict])
    return df
            
def result_loader(value, df_dict: dict):
    remove = ["", " ", "#"]
    value = value.split(" ")
    value = [l.strip() for l in value if l not in remove]
    measures, info = value[0:2], " ".join(value[2:])
    data = measures + [info]
    # print(data)
    columns = ["memory", "performance", "info"]
    df_dict.update(dict(zip(columns, data)))


def time_loader(key: str, value: str, df_dict: dict):
    df_dict[key + '_count'] = df_dict.get(key + '_count', 0) + 1
    df_dict[key] = df_dict.get(key, 0) + float(value.strip())



if __name__ == "__main__":
    df = read_experiment("blk_size_500_20230118_134540")
    print(df)