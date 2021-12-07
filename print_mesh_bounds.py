# Maps number x in the range [a, b] to number y in the range [c, d]
def affine_transform(x, a, b, c, d):
  return ((x - a) * ((d - c) / (b - a))) + c

with open(input("Filename: "), "r+") as file:
  lines = file.read().split("\n")

  low = [0, 0, 0]
  high = [0, 0, 0]

  for line in lines:
    if line.startswith("v "):
      for index, value in enumerate(line[2:].split(" ")):
        value = float(value)

        if low[index] > value:
          low[index] = value
        if high[index] < value:
          high[index] = value

  print(low)
  print(high)
