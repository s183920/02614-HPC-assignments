# Assignment 1

## Answers to questions

Essentially, the matrix-matrix multiplication algorithm consists of three nested
loops. In how many ways can the loops be nested?

The three loops can be nested in 6 ways:
    - nmk
    - nkm
    - mkn
    - mnk
    - kmn
    - knm

In which order do you expect the fastest algorithm to run?
    - The fastest algorithm is expected to run in the order nmk because the
      innermost loop is the one that iterates over the elements of the
      matrix. The innermost loop is the one that is most likely to be
      cached. The outermost loop is the one that is least likely to be
      cached. The middle loop is the one that is most likely to be
      cached, but not as much as the innermost loop.


FLOPS = 2mnk


## Mising implementation

- [ ] Blocked version experiment
- [ ] compilers experiment

### Plots
- [ ] Plot of size performance
- [ ] Plot of block size performance
- [ ] Plot of performance for different compilers


## Assumptions

- The cache is warmed up by the driver and the program is therefore only ran once for each size.
- The minimum runtime of 3s is not dislayed in the timing plot, which shows an average

## Compiler options to try

- [ ] -O3
- [ ] -Ofast
- [ ] -ftree-vectorize
- [ ] -funroll-all-loops
- [ ] -funsafe-loop-optimizations
- [ ] -funsafe-math-optimizations

Other
- [ ] -march=broadwell
- [ ] -mavx2
- [ ] -fprefetch-loop-arrays
- [ ] -restrict
- [ ] -flto
- [ ] -funroll-loops
- [ ] -march=broadwell -funroll-loops
