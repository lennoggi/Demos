#!/bin/bash

set -euo pipefail

ml rocm/6.3.2--spack--0.23.1
ml rocblas/6.3.2-gcc-10.5.0-hip-6.3.2-z7kf2et

hipcc rocblas_dgemm.hip -o rocblas_dgemm -I${ROCBLAS_INC} -L${ROCBLAS_LIB} -lrocblas

./rocblas_dgemm
