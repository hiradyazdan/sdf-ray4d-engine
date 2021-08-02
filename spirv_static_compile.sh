#!/usr/bin/env bash

if [[ -z ${STATIC_SHADERS_PATH} ]]; then
  echo "STATIC_SHADERS_PATH Variable should be set!"
  exit 1;
fi

if glslc 2>/dev/null; then
  glslc_cmd=glslc
else
  glslc_cmd=glslc.exe
fi

cd ${STATIC_SHADERS_PATH} || echo "Error - No such file or directory: ${STATIC_SHADERS_PATH}"

for ext in vert frag comp; do
  for file in *.${ext}; do
    if [[ ! -f $file ]]; then break; fi;

    prefix="$(cut -d'.' -f1 <<<"$file")"

    glslc_status=$(${glslc_cmd} $file -o ${prefix}.${ext}.spv)$?
    [[ $glslc_status = 0 ]] && echo "${prefix}.${ext}.spv was successfully created!"

    rm $file
  done
done

exit 0
