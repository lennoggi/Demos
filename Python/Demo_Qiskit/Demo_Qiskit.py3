import numpy as np
from IPython.display import display
from qiskit import __version__
from qiskit.quantum_info import Statevector
from qiskit.quantum_info import Operator
from qiskit.visualization import array_to_latex

print("")
print(f"Qiskit version: {__version__}")


# Classical states (column vectors)
ket0 = np.array([[1],[0]])
ket1 = np.array([[0],[1]])

print("")
print(0.5*(ket0 + ket1))


# Some operations (matrices)
M1 = np.array([[1, 1], [0, 0]])
M2 = np.array([[1, 0], [0, 1]])
M  = 0.5*(M1 + M2)

print("")
print(M)


# Some operations acting on the classical states or combined with each other
print("")
print(np.matmul(M1, ket1))
print(np.matmul(M1, M2))
print(np.matmul(M, M))


# Same as above, but with a nicer-looking formatting
# NOTE: will only display correctly in a Jupyter notebook
print("")
display(array_to_latex(np.matmul(M1, ket1)))
display(array_to_latex(np.matmul(M1, M2)))
display(array_to_latex(np.matmul(M, M)))


# Define quantum state vectors using Qiskit
one_over_sqrt2 = 1./np.sqrt(2.)

u = Statevector([one_over_sqrt2, one_over_sqrt2])
v = Statevector([(1. + 2.j)/3., -2./3.])
w = Statevector([1./3., 2./3.])

print("")
display(u.draw("text"))
display(u.draw("latex"))
print(u.draw("latex_source"))

print("")
print("u is " + ("valid (norm is 1)" if u.is_valid() else "not valid (norm is not 1)"))
print("v is " + ("valid (norm is 1)" if v.is_valid() else "not valid (norm is not 1)"))
print("w is " + ("valid (norm is 1)" if w.is_valid() else "not valid (norm is not 1)"))


# Measure quantum states
# NOTE: since measurements are probabilistic, running the script several times
#  will likely produce different results. Under the hood, this behavior is
#  probably implemented by drawing random numbers from an approppriate
#  distribution.
outcome, new_state = u.measure()
print(f"Measuring u produced {outcome} and modified u to {new_state}")
print("Old u is untouched: ", end = "")
display(u.draw("text"))

print("")
outcome, new_state = v.measure()
print(f"Measuring v produced {outcome} and modified v to {new_state}")
print("Old v is untouched: ", end = "")
display(v.draw("text"))

# This will throw an error, since w is not a valid quantum state
##outcome, new_state = w.measure()
##print(f"Measuring w produced {outcome} and modified w to {new_state}")
##print("Old w is untouched: ", end = "")
##display(w.draw("text"))


# Many measurements at once
nmeas = 100000
ustats = u.sample_counts(nmeas)
vstats = v.sample_counts(nmeas)
##wstats = w.sample_counts(nmeas)

print("")
print(f"Outcome of {nmeas} measurements of u: {ustats}")
print(f"Outcome of {nmeas} measurements of v: {vstats}")
##print(f"Outcome of {nmeas} measurements of w: {wstats}")


# Operators
Y = Operator([[0., -1.j],                       [1.j, 0]])
H = Operator([[one_over_sqrt2, one_over_sqrt2], [one_over_sqrt2, -one_over_sqrt2,]])
S = Operator([[1., 0.],                         [0., 1.j]])
T = Operator([[1., 0.],                         [0., (1. + 1.j)/np.sqrt(2.)]])

print("")
print(T.draw("text"))

ket0 = Statevector([1., 0.])
ket0 = ket0.evolve(H)
ket0 = ket0.evolve(T)
ket0 = ket0.evolve(H)
ket0 = ket0.evolve(S)
ket0 = ket0.evolve(Y)

print("")
print(ket0.draw("text"))


















