#! /bin/bash

# Launch with
# find . -name *.c -exec /home/hpompougnac/src/projects/LightPolyBenchC/commands.sh {} \;

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
fi

echo "Processing ${1}"

DIR="$(dirname "${1}")"
FILE="$(basename "${1}")"
new_path="$(echo "${DIR}/light_${FILE}")"
#
sed 's/DATA_TYPE POLYBENCH_2D(\([a-zA-Z]*\),\([A-Z]*\),\([A-Z]*\),[a-z]*,[a-z]*)/DATA_TYPE \1\[\2\]\[\3\]/g' $1 \
    | sed 's/DATA_TYPE POLYBENCH_3D(\([a-zA-Z]*\),\([A-Z]*\),\([A-Z]*\),\([A-Z]*\),[a-z]*,[a-z]*,[a-z]*)/DATA_TYPE \1\[\2\]\[\3\]\[\4\]/g' \
    | sed 's/DATA_TYPE POLYBENCH_1D(\([_a-z0-9]*\),\([A-Z]*\),[a-z]*)/DATA_TYPE \1\[\2\]/g' \
    | sed 's/POLYBENCH_2D_ARRAY_DECL(\([a-zA-Z0-9]*\),[[:space:]]*\([a-zA-Z_]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*[a-z]*,[[:space:]]*[a-z]*)/volatile \2 \1\[\3\]\[\4\]/g' \
    | sed 's/POLYBENCH_3D_ARRAY_DECL(\([a-zA-Z0-9]*\),[[:space:]]*\([A-Z_]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*[a-z]*,[[:space:]]*[a-z]*,[[:space:]]*[a-z]*)/volatile \2 \1\[\3\]\[\4\]\[\5\]/g' \
    | sed 's/POLYBENCH_1D_ARRAY_DECL(\([_a-zA-Z0-9]*\),[[:space:]]*\([a-zA-Z_]*\),[[:space:]]*\([A-Z]*\),[[:space:]]*[a-z]*)/volatile \2 \1\[\3\]/g' \
    | sed -z 's/  init_array[^;]*;//g' \
    | sed 's/polybench_stop.*//g' \
    | sed 's/polybench_print.*//g' \
    | sed 's/polybench_start.*//g' \
    | sed 's/POLYBENCH_FREE_ARRAY.*//g' \
    | sed 's/POLYBENCH_DUMP.*//g' \
    | sed 's/POLYBENCH_ARRAY(\([_A-Za-z0-9]*\))/\1/g' \
    | sed -z 's/\/\*[^*]*\*\///g' \
    | sed '/^[[:space:]]*$/d' \
    | sed 's/.*fprint.*//g' > $new_path

gcc -I utilities/ $new_path -lm 2> /dev/null || echo "- Compilation error"
rm -f a.out
echo ""
