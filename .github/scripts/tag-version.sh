# Initialize the version array with four zeros 
declare -a VERSION_ARRAY
IFS='.' read -ra VERSION_ARRAY <<< "0.0.0.0"

# Create a temporary file to store the updated version
temp_file=$(mktemp)

# Iterate through the git history, starting from the oldest commit 
git log --reverse --oneline --format="%h %s" | while read -r commit_hash commit_message; do 

  first_five=$(echo $commit_hash | cut -c 1-5)
  if git log --merges | grep -q "$first_five"; then
    # Check if the commit is NOT a merge commit (i.e. it's a merged commit)
    parents=$(git show --no-patch --format="%P" $commit_hash)
    if [[ $parents = *" "* ]]; then
      echo "Detected merge commit."
    else
      echo "Detected commit with more than one parent, continue."
      continue
    fi
  fi

  full_commit_message=$(git log -1 --pretty=format:"%B" "$commit_hash")

  while [[ $full_commit_message == *"Increments-api-version-of:"* ]]; do 
      ((VERSION_ARRAY[0]++)) 
      ((VERSION_ARRAY[1]=0)) 
      ((VERSION_ARRAY[2]=0)) 
      ((VERSION_ARRAY[3]=0)) 
      full_commit_message=${full_commit_message#*"Increments-api-version-of:"} 
  done

  full_commit_message=$(git log -1 --pretty=format:"%B" "$commit_hash")

  while [[ $full_commit_message == *"Increments-major-version-of:"* ]]; do 
      ((VERSION_ARRAY[1]++)) 
      ((VERSION_ARRAY[2]=0)) 
      ((VERSION_ARRAY[3]=0)) 
      full_commit_message=${full_commit_message#*"Increments-major-version-of:"} 
  done

  full_commit_message=$(git log -1 --pretty=format:"%B" "$commit_hash")

  while [[ $full_commit_message == *"Increments-minor-version-of:"* ]]; do 
      ((VERSION_ARRAY[2]++)) 
      ((VERSION_ARRAY[3]=0)) 
      full_commit_message=${full_commit_message#*"Increments-minor-version-of:"} 
  done

  full_commit_message=$(git log -1 --pretty=format:"%B" "$commit_hash")

  while [[ $full_commit_message == *"Increments-patch-version-of:"* ]]; do 
      ((VERSION_ARRAY[3]++)) 
      full_commit_message=${full_commit_message#*"Increments-patch-version-of:"} 
  done

  # Print the updated version to a temporary file
  echo "${VERSION_ARRAY[0]}.${VERSION_ARRAY[1]}.${VERSION_ARRAY[2]}.${VERSION_ARRAY[3]}" > "$temp_file"

done

# Read and print the updated version from the temporary file
cat "$temp_file"

# Read and parse the updated version from the temporary file as an array
IFS='.' read -ra TAG_VERSION_ARRAY < "$temp_file"

# Use the parsed array to form the tag_name
tag_name="${TAG_VERSION_ARRAY[0]}.${TAG_VERSION_ARRAY[1]}.${TAG_VERSION_ARRAY[2]}.${TAG_VERSION_ARRAY[3]}"

if git rev-parse -q --verify "refs/tags/$tag_name" > /dev/null; then
  echo "Tag '$tag_name' already exists."
else
  # Create the Git tag on the latest commit
  echo "Creating tag '$tag_name' on the latest commit."
  git tag "$tag_name"
  git push origin "$tag_name"
fi

rm "$temp_file"
