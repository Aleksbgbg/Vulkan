import os

CODE_PATHS = ["src", "test"]

root = os.path.dirname(os.path.realpath(__file__))

for path in CODE_PATHS:
  for subdir, dirs, files in os.walk(path):
    for file in files:
      if not file.endswith(".cpp") and not file.endswith(".cc") and not file.endswith(".h"):
        continue

      relative_path = os.path.join(subdir, file)

      with open(relative_path, "r+") as file_handle:
        lines = file_handle.read().splitlines()

        # Enforce trailing newline
        if len(lines[-1]) > 0:
          lines.append("")

        # Coerce header guards
        if file.endswith(".h"):
          header_guard = "VULKAN_" + relative_path.upper().replace("\\", "_")[:-2] + "_H_"        

          end_guard_line = 0

          for (index, line) in enumerate(lines):
            if line.startswith("#endif"):
              end_guard_line = index

          lines[0] = f"#ifndef {header_guard}"
          lines[1] = f"#define {header_guard}"
          lines[end_guard_line] = f"#endif  // {header_guard}"
          
        file_handle.seek(0)
        file_handle.write("\n".join(lines))
        file_handle.truncate()
      
      # .cpp -> .cc
      if file.endswith(".cpp"):
        os.rename(relative_path, relative_path[:-4] + ".cc")