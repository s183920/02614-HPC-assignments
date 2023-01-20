def read_file(file: Path) -> pd.DataFrame:
    df_dict = {}

    with file.open() as f:
        for line in f:
            lst = line.split(": ")
            if len(lst) < 2:
                df_dict["info"] = lst[0].strip()
            else:
                key, value = line.split(": ")
                if key in df_dict.keys():
                    
                    key = key + "_count"
                    if key in df_dict.keys():
                        df_dict[key] = df_dict[key] + 1
                    else:
                        df_dict[key] = 1
                else:
                    df_dict.update({key: value.strip()})