#!/bin/bash
# 02614 - High-Performance Computing, January 2022
# 
# batch script to run collect on a decidated server in the hpcintro
# queue
#
# Author: Bernd Dammann <bd@cc.dtu.dk>
#
# Note: to get more cores, change the n value below to the
#       number of cores you want to use.  Later on, use the
#       $LSB_DJOB_NUMPROC variable to use this number, e.g. in
#       export OMP_NUM_THREADS=$LSB_DJOB_NUMPROC
#
#BSUB -J collector
#BSUB -q hpcintro
#BSUB -n 20
#BSUB -W 15
#BSUB -R "rusage[mem=10GB]"


module load studio

# define the executable here
#
EXECUTABLE=poisson_j

# define any command line options for your executable here
EXECOPTS="700lf 5 0.01 0 0 2"

# set some OpenMP variables here
#
# no. of threads
export OMP_NUM_THREADS=$LSB_DJOB_NUMPROC
#
# keep idle threads spinning (needed to monitor idle times!)
export OMP_WAIT_POLICY=active
#
# if you use a runtime schedule, define it below
export OMP_SCHEDULE=static,1
export OMP_PLACES=cores
export OMP_PROC_BIND=spread


# experiment name 
#
JID=${LSB_JOBID}
EXPOUT="../er_tests/$LSB_JOBNAME.${JID}_static.er"
HWCOUNT = "-h dch,on,dcm,on,l2h,on,l2m,on,l3h,on,l3m,on"

# start the collect command with the above settings
collect -o $HWCOUNT $EXPOUT ./$EXECUTABLE $EXECOPTS
