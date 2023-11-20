#!/bin/bash

run_python_script() {
    folder=$1
    base_name=$2
    x=$3
    y=$4
    z=$5
    param=$6
    obj_file=$7
    echo "${folder}" "${base_name}" "${x},${y},${z}" "${param}"
    echo "${obj_file}"
    cd "${folder}"
    # Run the python script
    python ../build_Laberith.py "${base_name}" "${x},${y},${z}" "$param"
    python ../stl_to_obj.py "${base_name}" "${obj_file}"
    rm "${base_name}" 
    cd ..
}

for level in 1 2 3; do
    level_folder="level_${level}"
    mkdir -p "${level_folder}"
    length=$((50 * level))
    for i in $(seq 1 10); do
        x=$((RANDOM % 20))
        z=$((RANDOM % 20))
        # echo "${level_folder}" "laberinth_level${level}_${i}.stl" "${x}" 0 "${z}"  "${length}" "laberinth_level${level}_${i}.obj"
        run_python_script "$level_folder" "laberinth_level${level}_${i}.stl" "${x}" 0 "${z}" "${length}" "laberinth_level${level}_${i}.obj"
    done
done

