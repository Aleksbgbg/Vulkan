def getLines(t):
  print(t)
  line = ""

  while True:
    try:
      yield input()
    except EOFError:
      break
  
  return line

while True:
  linei = 0
  name = ""
  out = ""
  for line in getLines("code:\n"):
    if line.startswith("}"):
      print(out)
      break
    if line.startswith("typedef"):
      linei = 0
      name = ""

    if linei == 0:
      name = line.split()[2][2:] + "Builder"
      linei += 1
      continue

    parts = line.split()

    zeroi = 0

    t = ""

    if parts[0] == "const":
      zeroi += 1
      t += "const "

    t += parts[zeroi]

    n = parts[zeroi + 1][:-1]
    if n == "sType" or n == "pNext":
      continue
    if "uint32_t" in t:
      t = t.replace("uint32_t", "u32")
    if "int32_t" in t:
      t = t.replace("int32_t", "i32")
    nc = n[0].upper() + n[1:]
    out += f"  STRUCTURE_SETTER({name}, {nc}, {t}, {n})\n"
