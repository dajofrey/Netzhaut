#!/bin/bash

# Declare an empty array
files_to_process=()

# Run 'git diff --staged --name-only' and process each line
while IFS= read -r line; do
  # Check if the line contains the substring "src/lib/nh"
  if [[ $line == *src/lib/nh* ]]; then
    # Add the line to the array
    files_to_process+=("$line")
  fi
done < <(git diff --staged --name-only)

# Process the array elements
for ((i=0; i<${#files_to_process[@]}; i++)); do
  # Remove substring before the first occurrence of "nh"
  files_to_process[$i]=${files_to_process[$i]#*src/lib/}

  # Remove substring starting from the first occurrence of "/"
  files_to_process[$i]=${files_to_process[$i]%%/*}
done

# Create a new array with unique elements
unique_files=()
for file in "${files_to_process[@]}"; do
  if [[ ! " ${unique_files[@]} " =~ " $file " ]]; then
    unique_files+=("$file")
  fi
done

# Print the unique array elements
for unique_file in "${unique_files[@]}"; do
  echo "Detected changes to: $unique_file"
done

# Define the workload scope options
workload_options=("1: patch" "2: minor" "3: major" "4: api")

# Initialize an array to store selected workload scopes and corresponding actions
actions=()

# Needed inside Git hook
exec < /dev/tty

# Process each element in unique_files
for unique_file in "${unique_files[@]}"; do
  echo -e "\nWorkload in: $unique_file"
  echo "Workload scope options:"
  for option in "${workload_options[@]}"; do
    echo "$option"
  done

  while true; do
    read -p "Workload scope? (1-4): " workload_input
    case $workload_input in
      1|2|3|4)
        case $workload_input in
          1) workload_scope="patch" ;;
          2) workload_scope="minor" ;;
          3) workload_scope="major" ;;
          4) workload_scope="api" ;;
        esac
        echo "Selected workload scope: $workload_scope"
        actions+=("Increments-${workload_scope}-version-of: $unique_file")
        break
        ;;
      *)
        echo "Invalid input. Please enter a number from 1 to 4."
        ;;
    esac
  done
done

# Print the actions array
echo -e "\nResult:"
for action in "${actions[@]}"; do
  echo "$action"
done

# Check if a command-line argument was provided
if [ $# -eq 1 ]; then
  argfile="$1"
  # Append the actions to the specified file
  echo -e "\nAppending result to $argfile"
  for action in "${actions[@]}"; do
    echo "$action" >> "$argfile"
  done
fi

