import numpy as np
import pandas as pd

remove = ["", " ", "#"]

def get_data(exp, fname):
    data = pd.DataFrame(columns = ["memory", "performance", "info"])

    with open(exp + fname) as f:
        lines = f.readlines()
        
        for i, line in enumerate(lines):
            line = line.split(" ")
            line = [l.strip() for l in line if l not in remove]
            data.loc[i, "memory"] = line[0]
            data.loc[i, "performance"] = line[1]
            data.loc[i, "info"] = line[2:]
    
    data["memory"] = data["memory"].astype(float)
    data["performance"] = data["performance"].astype(float)

    return data

# setup data
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

def get_compiler_optimizations(exp, filename = "compile.log"):
    remove_flags = ["gcc", "-g", "-fPIC", "-o", "libmatmult.so", "-shared", "matrix.o", "-lm"]

    with open(exp + "compile_logs/" + filename, "r") as f:
        lines = f.readlines()
        assert len(lines) == 1
        line = lines[0]
        compile_flags = line.split(" ")
        compile_optimizations = [l.strip() for l in compile_flags if l.strip() not in remove_flags]
    return compile_optimizations