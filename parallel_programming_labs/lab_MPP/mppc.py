from ctypes import *
import time

so_file = "./func_c.so"
custom_functions = CDLL(so_file)

size_array = 10000
array = c_int * size_array
array_a = pointer(array(1 * 10000))
array_b = pointer(array(1 * 10000))
array_c = pointer(array(1 * 10000))

t_start = time.time()
custom_functions.compareArraysClear(array_a, array_b, array_c, size_array)
print("Function without vect and omp: ", (time.time() - t_start) * 1000)

t_start = time.time()
custom_functions.compareArraysVec(array_a, array_b, array_c, size_array)
print("Function with vec: ", (time.time() - t_start) * 1000)

t_start = time.time()
custom_functions.compareArraysOmp(array_a, array_b, array_c, size_array)
print("Function with omp: ", (time.time() - t_start) * 1000)

t_start = time.time()
custom_functions.compareArraysVecOmp(array_a, array_b, array_c, size_array)
print("Function with vec and omp: ", (time.time() - t_start) * 1000)
