TARGET	= libmatmult.so
LIBSRCS	= -lm
LIBOBJS	= matrix.o

OPT	= -g -O3
PIC	= -fPIC
XOPTS = 

CC	= gcc
CFLAGS= $(OPT) $(PIC) $(XOPTS)

SOFLAGS = -shared 
XLIBS	= 



$(TARGET): $(LIBOBJS)
	# @module load gcc
	$(CC) $(CFLAGS) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS) $(LIBSRCS)
	@echo $(CC) $(CFLAGS) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS) $(LIBSRCS) > compile.log

# load modules
loadmodules:
	@module load gcc
	
# cleaning functions
clean:
	@/bin/rm -f core core.* $(LIBOBJS) 

realclean: clean
	@/bin/rm -f $(TARGET)
	@/bin/rm -f compile.log

cleanresults:
	@/bin/rm -rf results/*

# dependencies
matrix.o: matrix.c

# save results
SAVEKEY="blk"
saveresults: 
	@mkdir -p results_saved
	@cp -r results/*$(SAVEKEY)* results_saved/
	# move hpc_log with jobid
	# @cp results/hpc_log results/hpc_log_$(SAVEKEY)
	# extract jobid from results/*$(SAVEKEY)*/jobid.txt
	
# test 
test: $(TARGET)
	@echo "Expected result: "
	@echo "83.594    151.173 0 # matmult_nat"
	@echo "Obtained result: "
	./matmult_c.gcc nat 50 60 70

# end of file