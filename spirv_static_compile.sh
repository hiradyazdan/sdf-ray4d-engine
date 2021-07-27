STATIC_SHADERS_PATH=assets/shaders/SDF/static

cd ${STATIC_SHADERS_PATH} || echo "Error - No such file or directory: ${STATIC_SHADERS_PATH}"

for f in *.vert
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${prefix}.vert.spv
done

for f in *.frag
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${prefix}.frag.spv
done

for f in *.comp
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${prefix}.comp.spv
done

rm *.vert *.frag *.comp
