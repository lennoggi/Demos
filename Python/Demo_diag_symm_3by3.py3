# Diagonalize a 3*3 real symmetric matrix using NumPy
import numpy as np

a11 = 5.
a12 = -2.
a13 = -4.
a22 = 8.
a23 = -2.
a33 = 5.

a = np.array([[a11, a12, a13],
              [a12, a22, a23],
              [a13, a23, a33]])

eigenvalues, eigenvectors = np.linalg.eigh(a)

print(eigenvalues)
print(eigenvectors)
