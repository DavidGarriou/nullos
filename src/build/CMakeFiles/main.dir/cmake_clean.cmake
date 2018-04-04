file(REMOVE_RECURSE
  "main.pdb"
  "main"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/main.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
