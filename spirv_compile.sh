shaders_path=assets/shaders

mkdir -p ${shaders_path}

for f in *.vert
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${shaders_path}/${prefix}.vert.spv
done

for f in *.frag
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${shaders_path}/${prefix}.frag.spv
done

for f in *.comp
do
    prefix="$(cut -d'.' -f1 <<<"$f")"
    glslc $f -o ${shaders_path}/${prefix}.comp.spv
done

rm *.vert *.frag *.comp
