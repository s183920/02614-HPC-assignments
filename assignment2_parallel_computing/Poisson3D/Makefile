# Makefile
#
TARGET_J  = poisson_j		# Jacobi
TARGET_GS = poisson_gs		# Gauss-Seidel

SOURCES	= main.c print.c alloc3d.c helper.c
OBJECTS	= print.o alloc3d.o helper.o
MAIN_J	= main_j.o
MAIN_GS = main_gs.o
OBJS_J	= $(MAIN_J) jacobi.o
OBJS_GS	= $(MAIN_GS) gauss_seidel.o

# options and settings for the GCC compilers
#
CC	= gcc
DEFS	= 
OPT	= -g -std=c99
IPO	= 
ISA	= 
CHIP	= 
ARCH	= 
PARA	= 
CFLAGS	= $(DEFS) $(ARCH) $(OPT) $(ISA) $(CHIP) $(IPO) $(PARA) $(XOPTS)
LDFLAGS = -lm 
TEST_FLAG =

all: realclean $(TARGET_J) $(TARGET_GS) 

$(TARGET_J): $(OBJECTS) $(OBJS_J)
	$(CC) -o $@ $(CFLAGS) $(OBJS_J) $(OBJECTS) $(LDFLAGS)

$(TARGET_GS): $(OBJECTS) $(OBJS_GS)
	$(CC) -o $@ $(CFLAGS) $(OBJS_GS) $(OBJECTS) $(LDFLAGS)

$(MAIN_J):
	$(CC) -o $@ -D_JACOBI $(TEST_FLAG) $(CFLAGS) -c main.c 

$(MAIN_GS):
	$(CC) -o $@ -D_GAUSS_SEIDEL $(TEST_FLAG) $(CFLAGS) -c main.c 

# test program
# $(TEST_MAIN_J):
# 	$(CC) -o $@ -D_JACOBI -D_TEST $(CFLAGS) -c main.c 

# $(TEST_MAIN_GS):
# 	$(CC) -o $@ -D_GAUSS_SEIDEL -D_TEST $(CFLAGS) -c main.c 

clean:
	@/bin/rm -f core *.o *~

realclean: clean
	@/bin/rm -f $(TARGET_J)  $(TARGET_GS)

test: TEST_FLAG += -D_TEST 
test: all
	@echo
	@echo "Running Jacobi test"
	@echo
	@echo "N = 20"
	./poisson_j 20 20000 0.0001 0
	@echo
	@echo "N = 50"
	./poisson_j 50 20000 0.0001 0

	@echo
	@echo "Running Gauss-Seidel test"
	@echo
	@echo "N = 20"
	./poisson_gs 20 20000 0.0001 0 3
	@echo
	@echo "N = 50"
	./poisson_gs 50 20000 0.0001 0

	@echo
# DO NOT DELETE

main_j.o: main.c print.h jacobi.h helper.h
main_gs.o: main.c print.h gauss_seidel.h helper.h
print.o: print.h
helper.o: helper.h
