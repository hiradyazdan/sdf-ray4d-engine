#!/usr/bin/env bash

if [[ -z ${STATIC_SHADERS_PATH} ]]; then
  echo "STATIC_SHADERS_PATH Variable should be set!"
  return 1
fi

if command -v glslc.exe >/dev/null 2>&1; then
  glslc_cmd=glslc.exe
else
  glslc_cmd=glslc
fi

cd ${STATIC_SHADERS_PATH} || return

for ext in vert frag comp; do
  for file in *.${ext}; do
    if [[ ! -f $file ]]; then continue; fi;

    prefix="$(cut -d'.' -f1 <<<"$file")"

    glslc_status=$(${glslc_cmd} $file -o ${prefix}.${ext}.spv)$?
    [[ $glslc_status = 0 ]] && echo -ne "${prefix}.${ext}.spv was successfully created!\n\r"

    rm $file
  done
done

return 0
