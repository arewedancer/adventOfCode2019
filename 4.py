def test(num):
  strn = str(num)
  d = False
  freq = {}
  for idx, s in enumerate(strn):
    if idx > 0 and s < strn[idx - 1]:
      return False
    if s in freq:
      freq[s]=freq[s] + 1
    else:
      freq[s] = 1
    #if idx > 0 and s == strn[idx - 1]:
    #  d = True
  for l in list(freq.values()):
    if l == 2:
      d = True
  return d


count = 0
for i in range(146810, 612565):
  if test(i):
    count = count + 1

print(count)

