file(REMOVE_RECURSE
  "nullos.pdb"
  "nullos"
)

# Per-language clean rules from dependency scanning.
foreach(lang ASM C)
  include(CMakeFiles/nullos.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
