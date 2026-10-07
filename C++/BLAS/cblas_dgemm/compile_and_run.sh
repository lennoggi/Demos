#!/bin/bash

set -euo pipefail

g++ cblas_dgemm.cc -o cblas_dgemm -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lopenblas

##ml openblas
####g++ cblas_dgemm.cc -o cblas_dgemm -I${OPENBLAS_INC} -L${OPENBLAS_LIB} -lopenblas
##g++ cblas_dgemm.cc -o cblas_dgemm -I${OPENBLAS_INC} -L. -lopenblas

./cblas_dgemm
