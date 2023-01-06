TARGET	= libmatmult.so
LIBSRCS	= -lm
LIBOBJS	= matrix.o

OPT	= -g -O3
PIC	= -fPIC # fpic necessary for shared libraries
XOPTS = 

CC	= gcc
CFLAGS= $(OPT) $(PIC) $(XOPTS)

SOFLAGS = -shared 
XLIBS	= 



$(TARGET): $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS) $(LIBSRCS)
	@echo $(CC) $(CFLAGS) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS) $(LIBSRCS) > compile.log

	
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
SAVEKEY="large"
saveresults: 
	@mkdir -p results_saved
	@cp -r results/*$(SAVEKEY)* results_saved/
	
# test 
test: $(TARGET)
	@echo "Expected result: "
	@echo "83.594    151.173 0 # matmult_nat"
	@echo "Obtained result: "
	./matmult_c.gcc nat 50 60 70

# end of file