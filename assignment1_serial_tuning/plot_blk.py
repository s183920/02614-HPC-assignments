import matplotlib.pyplot as plt
import pandas as pd

PATH = "results/matmult_test_20230105_095258/output_files/20.txt"

df = pd.read_csv(PATH, header=None)
df.columns = ["Memory", "MBit/s", "Description"]
print(df)
