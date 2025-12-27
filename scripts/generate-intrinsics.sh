#!/bin/bash

# Declare a string array
files=(
    "build/data/intrinsics/Object.is")

# Iterate through each string in the array
for f in "${files[@]}"; do
    filename=${f#*intrinsics/}
    new_string=$(echo "$f" | sed 's|.*/\([^/]*\)/\([^/]*\)\.is|\1_\2|')
    ./external/xxd/xxd -i $f src/lib/nh-ecmascript/Intrinsics/Templates/$filename.inc nh_ecmascript_${new_string}_p
done

# License notice
LICENSE_NOTICE="\
// LICENSE NOTICE ==================================================================================\n\n\
/**\n\
 * Netzhaut - Web Browser Engine\n\
 * Copyright (C) 2022  Dajo Frey\n\
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.\n\
 *\n\
 * This file was generated.\n\
 */\n\n"

# Output file
OUTPUT_FILE="src/lib/nh-ecmascript/Intrinsics/Templates.c"

# Function to extract the filename from the path
get_filename() {
    path=$1
    echo "${path#build/data/intrinsics/}"
}

# Generate the output file
generate_output() {
    # Write license notice
    echo -e "$LICENSE_NOTICE" > "$OUTPUT_FILE"

    # Write includes
    echo -e "// INCLUDE =========================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "#include \"Templates.h\"\n" >> "$OUTPUT_FILE"

    # Write file includes
    for file in "${files[@]}"; do
        echo "#include \"Templates/$(get_filename "$file").inc\"" >> "$OUTPUT_FILE"
    done

    echo -e "\n// FRAGMENT_NAMES ==================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "char *NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/intrinsics§/||; s|/|_|g; s/\.is$//')
        echo "    \"$new_name\"," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
    echo -e "size_t NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP_COUNT = sizeof(NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP) / sizeof(NH_ECMASCRIPT_INTRINSIC_TEMPLATE_NAMES_PP[0]);\n" >> "$OUTPUT_FILE"

    echo -e "// FRAGMENTS =======================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "unsigned char *NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/intrinsics/||; s|/|_|g; s/\.is$//')
        echo "    nh_ecmascript_intrinsics_${new_name}_p," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
    echo -e "size_t NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP_COUNT = sizeof(NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP) / sizeof(NH_ECMASCRIPT_INTRINSIC_TEMPLATES_PP[0]);\n" >> "$OUTPUT_FILE"

    echo -e "// FRAGMENT LENGTHS ================================================================================\n" >> "$OUTPUT_FILE"
    echo -e "const unsigned int NH_ECMASCRIPT_INTRINSIC_TEMPLATE_LENGTHS_P[] = {" >> "$OUTPUT_FILE"
    for file in "${files[@]}"; do
        new_name=$(echo "$file" | sed 's|build/data/intrinsics/||; s|/|_|g; s/\.is$//')
        echo "    nh_ecmascript_intrinsics_${new_name}_p_len," >> "$OUTPUT_FILE"
    done
    echo -e "};\n" >> "$OUTPUT_FILE"
}

# Call the function to generate the output
generate_output

# Print success message
echo "Output file '$OUTPUT_FILE' generated successfully."
