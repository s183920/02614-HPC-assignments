TARGET	= libmatmult.so
LIBSRCS	= -lm
LIBOBJS	= matrix.o

OPT	= -g
PIC	= -fPIC
XOPTS = 

CC	= gcc
CFLAGS= $(OPT) $(PIC) $(XOPTS)

SOFLAGS = -shared 
XLIBS	= 

$(TARGET): $(LIBOBJS)
	$(CC) -o $@ $(SOFLAGS) $(LIBOBJS) $(XLIBS)

clean:
	@/bin/rm -f core core.* $(LIBOBJS) 

realclean: clean
	@/bin/rm -f $(TARGET)

cleanresults:
	@/bin/rm -rf results/*

matrix.o: matrix.c

