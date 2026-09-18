#!/bin/bash
g++ cblas_dgemm.cc -o cblas_dgemm -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -lopenblas
./cblas_dgemm
