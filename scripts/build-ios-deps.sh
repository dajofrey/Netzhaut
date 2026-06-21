#!/bin/bash
set -e

ROOT_DIR=$(pwd)
EXT_IOS_DIR="$ROOT_DIR/external/ios"
SRC_DIR="$EXT_IOS_DIR/src"

FT_VERSION="2.13.2"
FT_DIR="$EXT_IOS_DIR/freetype"

HB_VERSION="8.3.0"
HB_DIR="$EXT_IOS_DIR/harfbuzz"

IOS_SDK=${1:-iphonesimulator}
IOS_MIN_VERSION="15.0"

# Determine target architecture and host for cross-compilation
if [ "$IOS_SDK" == "iphoneos" ]; then
    HOST="aarch64-apple-darwin"
    TARGET="arm64-apple-ios${IOS_MIN_VERSION}"
else
    HOST="aarch64-apple-darwin"
    TARGET="arm64-apple-ios${IOS_MIN_VERSION}-simulator"
fi

SDK_PATH=$(xcrun --sdk $IOS_SDK --show-sdk-path)

mkdir -p "$SRC_DIR"

# ==========================================
# 1. BUILD FREETYPE
# ==========================================
echo "=== Building FreeType for $IOS_SDK ==="
mkdir -p "$FT_DIR/lib" "$FT_DIR/include"

if [ ! -f "$SRC_DIR/freetype-$FT_VERSION.tar.gz" ]; then
    curl -L "https://download.savannah.gnu.org/releases/freetype/freetype-$FT_VERSION.tar.gz" -o "$SRC_DIR/freetype-$FT_VERSION.tar.gz"
fi

cd "$SRC_DIR"
tar -xzf "freetype-$FT_VERSION.tar.gz"
cd "freetype-$FT_VERSION"

./configure \
  --host=$HOST \
  --without-zlib \
  --without-png \
  --without-bzip2 \
  --without-harfbuzz \
  CFLAGS="-arch arm64 -isysroot $SDK_PATH -target $TARGET -O3"

make -j$(sysctl -n hw.ncpu)
cp objs/.libs/libfreetype.a "$FT_DIR/lib/"
cp -r include/* "$FT_DIR/include/"

# ==========================================
# 2. BUILD HARFBUZZ
# ==========================================
echo "=== Building HarfBuzz for $IOS_SDK ==="
cd "$SRC_DIR"

if [ ! -f "$SRC_DIR/harfbuzz-$HB_VERSION.tar.xz" ]; then
    curl -L "https://github.com/harfbuzz/harfbuzz/releases/download/$HB_VERSION/harfbuzz-$HB_VERSION.tar.xz" -o "$SRC_DIR/harfbuzz-$HB_VERSION.tar.xz"
fi

tar -xf "harfbuzz-$HB_VERSION.tar.xz"
cd "harfbuzz-$HB_VERSION"

# Use CMake to cross-compile for iOS and link against our custom FreeType
cmake -B build \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT="$SDK_PATH" \
  -DCMAKE_OSX_ARCHITECTURES="arm64" \
  -DCMAKE_INSTALL_PREFIX="$HB_DIR" \
  -DBUILD_SHARED_LIBS=OFF \
  -DHB_HAVE_FREETYPE=ON \
  -DFREETYPE_INCLUDE_DIR_ft2build="$FT_DIR/include" \
  -DFREETYPE_INCLUDE_DIR_freetype2="$FT_DIR/include" \
  -DFREETYPE_LIBRARY="$FT_DIR/lib/libfreetype.a" \
  -DHB_HAVE_CORETEXT=ON \
  -DHB_BUILD_TESTS=OFF

cmake --build build --config Release -j$(sysctl -n hw.ncpu)
cmake --install build --config Release

echo "=== Dependencies build complete ==="
