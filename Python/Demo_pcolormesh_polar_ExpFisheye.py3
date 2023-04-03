import numpy as np
from matplotlib import pyplot as plt
import argparse


# Parser setup
description = ("Generate a polar plot of some random data using pcolormesh and "
               "an exponentially increasing cell size along the radial direction")
parser = argparse.ArgumentParser(description = description)

parser.add_argument("rmin", type = float, metavar = "<rmin>",
                    help = "Minimum radius")
parser.add_argument("rmax", type = float, metavar = "<rmax>",
                    help = "Maximum radius")
parser.add_argument("nr", type = int, metavar = "<nr>",
                    help = "Number of points along r")
parser.add_argument("nphi", type = int, metavar = "<nphi>",
                    help = "Number of points along phi")


# Unpack the input
args = parser.parse_args()
rmin = args.rmin
rmax = args.rmax
nr   = args.nr
nphi = args.nphi


# Set up coordinates and generate the mesh
# NOTE: there are:
#       - nr   cells (-> nr+1   edges) along r
#       - nphi cells (-> nphi+1 edges) along phi
logr = np.linspace(np.log(rmin), np.log(rmax), nr + 1)
phi  = np.linspace(0., 2.*np.pi, nphi + 1)

mlogr, mphi = np.meshgrid(logr, phi)

x = np.exp(mlogr)*np.cos(mphi)
y = np.exp(mlogr)*np.sin(mphi)


# Generate some random data with the desired dimensionality
# NOTE: 1. shape is (nphi, nr), not (nr, nphi), because r is on the horizontal
#          direction (columns) and phi is on the vertical direction (rows)
#       2. shape is (nphi, nr), not (nphi+1, nr+1), bacause there are nr cells
#          along r and nphi cless along phi
data = np.random.rand(nphi, nr)

# Plot
plt.figure(figsize = [10, 10], dpi = 200)
plt.pcolormesh(x, y, data)
plt.tight_layout()
plt.savefig("Demo_pcolormesh_polar_ExpFisheye.pdf")
plt.close()
