# Compiling and running on Frontera

## Parallel version
```
mpicxx Demo_PHDF5_Cactus_read.cc -o Demo_PHDF5_Cactus_read -I /opt/apps/intel19/impi19_0/phdf5/1.12.2/x86_64/include -L /opt/apps/intel19/impi19_0/phdf5/1.12.2/x86_64/lib -lhdf5
sbatch Frontera.sub
```

## Serial version
The following assumes the module `hdf5/1.10.4` is loaded. If another version of HDF5 is loaded, then change the include and link paths accordingly or runtime errors will likely occur. No need to bother about this with the parallel version of the code, since it is run on a compute node and `phdf5/1.12.2` is loaded there (see `Frontera.sub`).
```
icpc Demo_PHDF5_Cactus_read_serial.cc -o Demo_PHDF5_Cactus_read_serial -I /opt/apps/intel19/hdf5/1.10.4/x86_64/include -L /opt/apps/intel19/hdf5/1.10.4/x86_64/lib -lhdf5
./Demo_PHDF5_Cactus_read_serial
```
