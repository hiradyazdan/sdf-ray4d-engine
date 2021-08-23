#!/usr/bin/env bash

if [[ -z ${PARTS_SHADERS_PATH} ]]; then
  echo "PARTS_SHADERS_PATH Variable should be set!"
  exit 1;
fi

if glslc 2>/dev/null; then
  glslc_cmd=glslc
else
  glslc_cmd=glslc.exe
fi

cd ${PARTS_SHADERS_PATH} || echo "Error - No such file or directory: ${PARTS_SHADERS_PATH}"

sdfr_pass_file_copy=sdfr_pass.frag
sdfr_pass_file_orig=../dynamic/sdfr_pass.frag

GLSL_FILE_COUNT=2
counter=0
for ext in glsl vert frag comp; do
  for file in *.${ext}; do
    if [[ ! -f $file ]]; then break; fi;

    if [[ ${ext} == glsl && $counter < "$GLSL_FILE_COUNT" ]]; then
      counter+=1
      cat $file >> $sdfr_pass_file_copy
      continue
    fi

    if [[ $sdfr_pass_file_orig ]]; then
      cat $sdfr_pass_file_orig >> $sdfr_pass_file_copy
      sdfr_pass_file_orig=
    fi

    if [[ $sdfr_pass_file_copy ]]; then
      sed -i 's/#version/\/\/#version/g' $sdfr_pass_file_copy
      sed -i '0,/^/s//#version 450\n/' $sdfr_pass_file_copy
    fi

    prefix="$(cut -d'.' -f1 <<<"$file")"

    glslc_status=$(${glslc_cmd} $file -o ${prefix}.${ext}.spv)$?
    [[ $glslc_status = 0 ]] && echo "${prefix}.${ext}.spv was successfully created!"

    if [[ "$prefix.$ext" == "$sdfr_pass_file_copy" ]]; then
      mv ${prefix}.${ext}.spv ../static
    fi

    rm $file
    sdfr_pass_file_copy=
  done
done

exit 0
