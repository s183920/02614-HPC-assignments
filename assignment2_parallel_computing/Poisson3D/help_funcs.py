import numpy as np

def read_binary(filename):
    x = np.fromfile(filename, dtype=float)
    N = np.cbrt(x.shape[0])
    x = x.reshape(int(N), int(N), int(N))

    return x


# x = read_binary("poisson_res_5.bin")
u = read_binary("u.txt")
u_true = read_binary("u_true.txt")

print(np.mean(u - u_true))