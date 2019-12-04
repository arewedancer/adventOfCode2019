import numpy as np

def get_fuel(mod):
  ret = 0
  module = mod[0]
  while module > 0:
    module = module // 3 - 2
    if module > 0:
      ret = ret + module
  return ret


mo_array = np.genfromtxt("data/1.txt", dtype="int32").reshape((100,1))
ret_array = np.apply_along_axis(get_fuel, 1, mo_array)
sum = ret_array.sum()
print(sum)
