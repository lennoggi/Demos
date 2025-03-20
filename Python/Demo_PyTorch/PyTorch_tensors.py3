# =====================================================================
# See https://pytorch.org/tutorials/beginner/blitz/tensor_tutorial.html
# =====================================================================

import numpy as np
import torch

# A tensor is an extension of Python's tuples and lists and NumPy ndarrays that
# can run on GPUs and other accelerators (if available)
arr   = (1, 2, 3, 4, 5)    # This could also be a list [] or a NumPy array
arr_t = torch.tensor(arr)

print(arr_t)
print(arr_t.shape)
print(arr_t.dtype)
print(arr_t.device)


# Move the tensor to the GPU if available
if torch.cuda.is_available():
    arr_t = arr_t.to("cuda")
    print("Device tensor stored on " + arr_t.device)


# Tensors can be converted to NumPy arrays
arr_np = arr_t.numpy()
print(arr_np)
