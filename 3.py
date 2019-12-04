import numpy as np


def mark(step, line, x, y):
  direction = step[0]
  volume = int(step[1:])
  distance = 18000
  if direction == 'R':
    for i in range(x, x + volume + 1):
      if square[i, y] == 1 and line == 2 and i != central_x:
        distance = min(distance, abs(i - central_x) + abs(y - central_y))
    square[x:x + volume, y] = line
    x = x + volume
  elif direction == 'L':
    for i in range(x - volume, x + 1):
      if square[i, y] == 1 and line == 2 and i != central_x:
        distance = min(distance, abs(i - central_x) + abs(y - central_y))
    square[x: x - volume, y] = line
    x = x - volume
  elif direction == 'U':
    for i in range(y - volume, y + 1):
      if square[x, i] == 1 and line == 2 and y != central_y:
        distance = min(distance, abs(i - central_y) + abs(x - central_x))
    square[x, y:y - volume] = line
    y = y - volume
  elif direction == 'D':
    for i in range(y, y + volume + 1):
      if square[x, i] == 1 and line == 2 and y != central_y:
        distance = min(distance, abs(i - central_y) + abs(x - central_x))
    square[x, y:y + volume] = line
    y = y + volume

  return x, y, distance


square = np.zeros((18000, 18000))
central_x = 9000
central_y = 9000
ret = 18000
with open("data/3.txt", 'r') as f:
  for cnt, line in enumerate(f):
    l = line.split(',')
    x = central_x
    y = central_y
    for step in l:
      x_, y_, dist = mark(step, cnt + 1, x, y)
      ret = min(ret, dist)
      x = x_
      y = y_

print(ret)
