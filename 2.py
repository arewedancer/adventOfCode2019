
def compute(input):
  current = 0
  op = -1
  while True:
    op = input[current]
    if op == 99:
      break
    elif op == 1:
      p1 = input[current + 1]
      p2 = input[current + 2]
      p3 = input[current + 3]
      input[p3] = input[p1] + input[p2]
    elif op == 2:
      p1 = input[current + 1]
      p2 = input[current + 2]
      p3 = input[current + 3]
      input[p3] = input[p1] * input[p2]
    else:
      print("wrong op")
    current = current + 4
  return input[0]


with open("data/2.txt", 'r') as f:
  tmp = f.readline()

input = [int(x) for x in tmp.split(',')]

for i in range(0, 99):
  for j in range(0, 99):
    f = input.copy()
    f[1] = i
    f[2] = j
    if compute(f) == 19690720:
      print (100*i + j)
      exit(0)



