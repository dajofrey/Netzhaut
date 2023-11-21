#!/bin/bash

if [ "$1" = "clean" ]; then
    rm -f src/lib/nhencoding/Base/UnicodeData.gen.c
    rm -f src/lib/nhencoding/Base/UnicodeData.gen.h
else
    # License Notice
    LICENSE_NOTICE="\
    /**\n\
     * Netzhaut - Web Browser Engine\n\
     * Copyright (C) 2022  Dajo Frey\n\
     * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.\n\
     */\n"
    
    # Define input/output file
    input_file="build/data/misc/UnicodeData-14.0.0d3.txt"
    c_file="src/lib/nhencoding/Base/UnicodeData.gen.c"
    h_file="src/lib/nhencoding/Base/UnicodeData.gen.h"
    
    # Write License Notice
    echo -e "// LICENSE NOTICE ==================================================================================\n" > "$c_file"
    echo -e "$LICENSE_NOTICE" >> "$c_file"
    echo -e "// INCLUDE =========================================================================================\n\n#include \"UnicodeData.gen.h\"\n" >> "$c_file"
    echo -e "// LOOKUP ==========================================================================================\n" >> "$c_file"
    
    # Count the number of lines in the input file
    num_lines=$(wc -l < "$input_file")
    
    # Start the string array definition
    echo "const NH_BYTE NH_ENCODING_UNICODE_DATA_PP[$num_lines][255] = {" >> "$c_file"
    
    # Initialize variables
    tuple_index=0
    previous_codepoint=0
    
    # Read each line from the input file and process it
    while IFS= read -r line; do
        # Add the line to the string array in the output file
        echo "    \"$line\"," >> "$c_file"
    done < "$input_file"
    
    # End the string array definition
    echo "};" >> "$c_file"
    
    echo -e "\n// UNDEFINED RANGES ================================================================================\n" >> "$c_file"
    
    # Get number of tuples
    tuples=0
    tuple_index=0
    previous_codepoint=0
    
    while IFS= read -r line; do
        # Extract the hex number from the beginning of the line until the first ";"
        current_codepoint=$(echo "$line" | cut -d';' -f1)
        current_codepoint=$((16#$current_codepoint))
        # Compare the current and previous codepoints
        if [ $tuple_index -eq 0 ]; then
            # Skip the first line to have a valid previous_codepoint
            tuple_index=$((tuple_index + 1))
        else
            if [ $((current_codepoint - previous_codepoint)) -gt 1 ]; then
                tuples=$((tuples + 1))
            fi
        fi
        previous_codepoint=$current_codepoint
    done < "$input_file"
    
    # Start the tuple array definition
    echo "const long NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[$tuples][2] = {" >> "$c_file"
    
    # Reset variables for the tuple array
    tuple_index=0
    previous_codepoint=0
    
    # Read each line from the input file and process it for the tuple array
    while IFS= read -r line; do
        # Extract the hex number from the beginning of the line until the first ";"
        current_codepoint=$(echo "$line" | cut -d';' -f1)
        current_codepoint=$((16#$current_codepoint))
    
        # Compare the current and previous codepoints
        if [ $tuple_index -eq 0 ]; then
            # Skip the first line to have a valid previous_codepoint
            tuple_index=$((tuple_index + 1))
        else
            if [ $((current_codepoint - previous_codepoint)) -gt 1 ]; then
                # Add a tuple to the tuple array in the output file
                incremented_codepoint=$((previous_codepoint + 1))
                decremented_codepoint=$((current_codepoint - 1))
                echo "    {$incremented_codepoint, $decremented_codepoint}," >> "$c_file"
            fi
        fi
    
        # Update the previous codepoint
        previous_codepoint=$current_codepoint
    
    done < "$input_file"
    
    # End the tuple array definition
    echo "};" >> "$c_file"
    
    echo -e "#ifndef NH_ENCODING_UNICODE_DATA_H" > "$h_file"
    echo -e "#define NH_ENCODING_UNICODE_DATA_H\n" >> "$h_file"
 
    echo "#ifndef DOXYGEN_SHOULD_SKIP_THIS\n" >> "$h_file"
    echo -e "$LICENSE_NOTICE" >> "$h_file"
    echo -e "#include \"../Common/Includes.h\"\n#endif\n" >> "$h_file"
    
    echo -e "    #define NH_ENCODING_UNICODE_DATA_COUNT $num_lines" >> "$h_file"
    echo -e "    #define NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_COUNT $tuples" >> "$h_file"
    echo -e "    extern const NH_BYTE NH_ENCODING_UNICODE_DATA_PP[$num_lines][255];" >> "$h_file"
    echo -e "    extern const long NH_ENCODING_UNICODE_DATA_UNDEFINED_RANGES_PP[$tuples][2];\n#endif" >> "$h_file"

    echo "Script completed. Output written to $c_file and $h_file."
fi
