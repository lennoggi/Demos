import numpy as np
from matplotlib import pyplot as plt
from matplotlib import colors as colors
from postcactus.simdir import SimDir
from postcactus import grid_data as gd


######################### USER-DEFINED PARAMETERS ##############################

data_dir = "/lagoon/bbhdisk/BBHDiskMerger/CBD_handoff_IGM_McLachlan_Spinning_aligned08_RadCool_OrbSep10M"

plots_dir = "/lagoon/lennoggi/Snapshots"
ext       = ".pdf"

gf    = "rho_b"
plane = "xy"
it    = 0

plot_extent = [-20., 20., -20., 20.]
npoints     = [200, 200]

logscale    = True
symlogscale = False
linnorm     = True

vmin = 1.e-08
vmax = 1.25e-2

cmap = "plasma"

figsize = [10., 10.]
dpi     = 200

################################################################################


#  Load the data
sd = SimDir(data_dir)


# Unpack the grid parameters
xmin = plot_extent[0]
xmax = plot_extent[1]
ymin = plot_extent[2]
ymax = plot_extent[3]

nx = npoints[0]
ny = npoints[1]


# Build the desired geometry
g = gd.RegGeom([nx, ny], [xmin, ymin], x1 = [xmax, ymax])


# Select the plane
if (plane == "xy"):
    read_data = sd.grid.xy.read
elif (plane == "xz"):
    read_data = sd.grid.xz.read
elif (plane == "yz"):
    read_data = sd.grid.yz.read
else:
    raise RuntimeError("Invalid plane \"" + plane + "\"")


# Build the patch
patch = read_data(gf, it, geom = g,
                  adjust_spacing = True, order = 0,
                  outside_val = 0., level_fill = False)

# NOTE: the 'adjust_spacing = True' option above resizes the 2D NumPy array
#       representing the grid data by snapping to the finest available
#       resolution. Therefore, nx and ny must be adjusted in order to match the
#       situation.
data   = np.transpose(patch.data)
nx_new = data.shape[0]
ny_new = data.shape[1]
print("nx and ny were modified from (" + str(nx) + ", " + str(ny) + ") to " + str(nx_new) + ", " + str(ny_new) + ")")


# Generate the mesh
# NOTE: nx cells => nx+1 edges/vertices (same for y)
x = np.linspace(xmin, xmax, nx_new + 1)
y = np.linspace(ymin, ymax, ny_new + 1)
mx, my = np.meshgrid(x, y)

# Set up the normalization
if (logscale):
    if (symlogscale):
        norm = colors.SymLogNorm(vmin = vmin, vmax = vmax, linthresh = vmin)
    else:
        norm = colors.LogNorm(vmin = vmin, vmax = vmax)
elif (linnorm):
    norm = colors.Normalize(vmin = vmin, vmax = vmax)
else:
    norm = None


# Plot
plt.figure(figsize = figsize, dpi = dpi)
plt.pcolormesh(mx, my, np.transpose(patch.data),
               shading = "auto", cmap = cmap, norm = norm)
plt.tight_layout()
plt.savefig(plots_dir + "/" + gf + ext)
plt.close()
