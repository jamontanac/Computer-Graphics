#!/bin/bash

run_python_script() {
    folder=$1
    base_name=$2
    x=$3
    y=$4
    z=$5
    param=$6
    obj_file=$7
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
    for i in $(seq 1 3); do
        if [ "$i" -eq 1 ]; then
            # When i=1, set x and z to 0
            x=0
            z=0
        else
            # For other values of i, use random x and z
            x=$(( (RANDOM % 10)- 10 ))
            x=$(( (x * 15) ))
            z=$(( (RANDOM % 10)-10 ))
            z=$(( (z * 15) ))
        fi
        run_python_script "$level_folder" "laberinth_level${level}_${i}.stl" "${x}" 0 "${z}" "${length}" "laberinth_level${level}_${i}.obj"
    done
done
mv level_1/*.obj ../src/LevelZero/resources
mv level_2/*.obj ../src/LevelOne/resources
mv level_3/*.obj ../src/LevelTwo/resources
rm -r level_*

