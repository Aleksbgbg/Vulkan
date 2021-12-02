# Create setters for every field on a Vulkan struct as it appears in the
# documentation, to be used with the macros that define a Vulkan struct builder.

def read_struct_lines(prompt):
  print(prompt)
  line = ""

  while True:
    try:
      yield input()
    except EOFError:
      break
  
  return line

while True:
  line_index = 0
  struct_name = ""
  builder_code = "\n"

  for line in read_struct_lines("Enter struct:\n"):
    if line.startswith("}"):
      print(builder_code)
      break

    if line.startswith("//"):
      continue

    if line.startswith("typedef"):
      line_index = 0
      struct_name = ""

    if line_index == 0:
      struct_name = line.split()[2][2:] + "Builder"
      line_index += 1
      continue

    parts = line.split()

    data_type_index = 0
    data_type = ""

    is_pointer = parts[0] == "const"

    if is_pointer:
      data_type_index += 1
      data_type += "const "

    data_type += parts[data_type_index]
    field_name = parts[data_type_index + 1][:-1]

    if is_pointer:
      data_type += " const"
    
    if field_name == "sType" or field_name == "pNext":
      continue
    
    if "uint32_t" in data_type:
      data_type = data_type.replace("uint32_t", "u32")
    
    if "int32_t" in data_type:
      data_type = data_type.replace("int32_t", "i32")

    if data_type_index == 0:
      data_type = "const " + data_type

    setter_name = field_name[0].upper() + field_name[1:]
    builder_code += f"  STRUCTURE_SETTER({setter_name}, {data_type}, {field_name})\n"
