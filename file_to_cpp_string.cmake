file(READ ${INPUT_PATH} PAGE_CONTENT)
set(PAGE_CONTENT "R\"STRING(${PAGE_CONTENT})STRING\"")
file(WRITE ${OUTPUT_PATH} ${PAGE_CONTENT})
