if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <name>"
    exit 1
fi

TMP="0.0.0.0"
declare -a     VERSION_ARRAY
declare -a OLD_VERSION_ARRAY

# List Git tags from oldest to newest
TAGS=$(git tag --sort=-creatordate)

# Check if any tag matches the pattern "$1"
for TAG in $TAGS; do
  if [[ "$TAG" == $1* && !(-z $1) ]]; then
    # Extract version information after "$1"
    TMP=${TAG#$1.}
    echo "Matching tag found: $TAG, Extracted version: $TMP"
    break
  fi
done

# Split TMP into an array containing 4 ints
IFS='.' read -ra     VERSION_ARRAY <<< "$TMP"
IFS='.' read -ra OLD_VERSION_ARRAY <<< "$TMP"

# Check commit message and increment patch version if needed
COMMIT_MSG=$(git log --format=%B -n 1)
if [[ "$COMMIT_MSG" == *"Increments-patch-version-of: $1"* ]]; then
  ((VERSION_ARRAY[3]++))
  echo "Match found. Incrementing the 4th element."
else
  echo "No match found."
fi

# Check commit message and increment minor version if needed
COMMIT_MSG=$(git log --format=%B -n 1)
if [[ "$COMMIT_MSG" == *"Increments-minor-version-of: $1"* ]]; then
  ((VERSION_ARRAY[3]=0))
  ((VERSION_ARRAY[2]++))
  echo "Match found. Incrementing the 3rd element, zeroing the 4th element."
else
  echo "No match found."
fi

# Check commit message and increment major version if needed
COMMIT_MSG=$(git log --format=%B -n 1)
if [[ "$COMMIT_MSG" == *"Increments-major-version-of: $1"* ]]; then
  ((VERSION_ARRAY[3]=0))
  ((VERSION_ARRAY[2]=0))
  ((VERSION_ARRAY[1]++))
  echo "Match found. Incrementing the 2nd element, zeroing the 3rd and 4th element."
else
  echo "No match found."
fi

# Check commit message and increment api version if needed
COMMIT_MSG=$(git log --format=%B -n 1)
if [[ "$COMMIT_MSG" == *"Increments-api-version-of: $1"* ]]; then
  ((VERSION_ARRAY[3]=0))
  ((VERSION_ARRAY[2]=0))
  ((VERSION_ARRAY[1]=0))
  ((VERSION_ARRAY[0]++))
  echo "Match found. Incrementing the 1st element, zeroing the 2nd, 3rd and 4th element."
else
  echo "No match found."
fi

# Compare versions and push new tag if necessary
if [[ "${VERSION_ARRAY[@]}" != "${OLD_VERSION_ARRAY[@]}" ]]; then
  TAG_NAME="$1"
  for element in "${VERSION_ARRAY[@]}"; do
    TAG_NAME+=".$element"
  done
  echo "Creating new tag $TAG_NAME"
else
  echo "No change in version array. Not creating a new tag."
fi
