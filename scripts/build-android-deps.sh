#!/bin/bash
set -e

ROOT_DIR=$(pwd)
EXT_ANDROID_DIR="$ROOT_DIR/external/android"
SRC_DIR="$EXT_ANDROID_DIR/src"

FT_VERSION="2.13.2"
FT_DIR="$EXT_ANDROID_DIR/freetype"

HB_VERSION="8.3.0"
HB_DIR="$EXT_ANDROID_DIR/harfbuzz"

# Locate Android NDK
ANDROID_NDK="${ANDROID_NDK_HOME:-$ANDROID_NDK_ROOT}"
if [ -z "$ANDROID_NDK" ]; then
    echo "Error: Set ANDROID_NDK_HOME or ANDROID_NDK_ROOT to your Android NDK path." >&2
    exit 1
fi

ANDROID_ABI=${1:-arm64-v8a}
ANDROID_API=${2:-24}

# Determine host tag
if [ "$(uname -s)" = "Darwin" ]; then
    HOST_TAG="darwin-x86_64"
else
    HOST_TAG="linux-x86_64"
fi

TOOLCHAIN_BIN="$ANDROID_NDK/toolchains/llvm/prebuilt/$HOST_TAG/bin"

# Target and compiler configuration based on ABI
case "$ANDROID_ABI" in
    arm64-v8a)
        TARGET="aarch64-linux-android"
        CC_NAME="${TARGET}${ANDROID_API}-clang"
        ;;
    armeabi-v7a)
        TARGET="armv7a-linux-androideabi"
        CC_NAME="${TARGET}${ANDROID_API}-clang"
        ;;
    x86_64)
        TARGET="x86_64-linux-android"
        CC_NAME="${TARGET}${ANDROID_API}-clang"
        ;;
    x86)
        TARGET="i686-linux-android"
        CC_NAME="${TARGET}${ANDROID_API}-clang"
        ;;
    *)
        echo "Error: Unsupported ANDROID_ABI '$ANDROID_ABI'" >&2
        exit 1
        ;;
esac

CC="$TOOLCHAIN_BIN/$CC_NAME"
AR="$TOOLCHAIN_BIN/llvm-ar"
RANLIB="$TOOLCHAIN_BIN/llvm-ranlib"

mkdir -p "$SRC_DIR"

# Determine parallel build jobs
if command -v nproc &> /dev/null; then
    JOBS=$(nproc)
elif command -v sysctl &> /dev/null; then
    JOBS=$(sysctl -n hw.ncpu)
else
    JOBS=4
fi

# ==========================================
# 1. BUILD FREETYPE
# ==========================================
echo "=== Building FreeType for Android ($ANDROID_ABI, API $ANDROID_API) ==="
mkdir -p "$FT_DIR/lib" "$FT_DIR/include"

if [ ! -f "$SRC_DIR/freetype-$FT_VERSION.tar.gz" ]; then
    curl -L "https://download.savannah.gnu.org/releases/freetype/freetype-$FT_VERSION.tar.gz" -o "$SRC_DIR/freetype-$FT_VERSION.tar.gz"
fi

cd "$SRC_DIR"
rm -rf "freetype-$FT_VERSION"
tar -xzf "freetype-$FT_VERSION.tar.gz"
cd "freetype-$FT_VERSION"

CC="$CC" AR="$AR" RANLIB="$RANLIB" \
./configure \
  --host="$TARGET" \
  --without-zlib \
  --without-png \
  --without-bzip2 \
  --without-harfbuzz \
  --enable-static \
  --disable-shared

make -j$JOBS
cp objs/.libs/libfreetype.a "$FT_DIR/lib/"
cp -r include/* "$FT_DIR/include/"

# ==========================================
# 2. BUILD HARFBUZZ
# ==========================================
echo "=== Building HarfBuzz for Android ($ANDROID_ABI, API $ANDROID_API) ==="
cd "$SRC_DIR"

if [ ! -f "$SRC_DIR/harfbuzz-$HB_VERSION.tar.xz" ]; then
    curl -L "https://github.com/harfbuzz/harfbuzz/releases/download/$HB_VERSION/harfbuzz-$HB_VERSION.tar.xz" -o "$SRC_DIR/harfbuzz-$HB_VERSION.tar.xz"
fi

rm -rf "harfbuzz-$HB_VERSION"
tar -xf "harfbuzz-$HB_VERSION.tar.xz"
cd "harfbuzz-$HB_VERSION"

cmake -B build \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_SYSTEM_VERSION="$ANDROID_API" \
  -DCMAKE_ANDROID_ARCH_ABI="$ANDROID_ABI" \
  -DCMAKE_ANDROID_NDK="$ANDROID_NDK" \
  -DCMAKE_ANDROID_STL_TYPE=c++_static \
  -DCMAKE_INSTALL_PREFIX="$HB_DIR" \
  -DBUILD_SHARED_LIBS=OFF \
  -DHB_HAVE_FREETYPE=ON \
  -DFREETYPE_INCLUDE_DIR_ft2build="$FT_DIR/include" \
  -DFREETYPE_INCLUDE_DIR_freetype2="$FT_DIR/include" \
  -DFREETYPE_LIBRARY="$FT_DIR/lib/libfreetype.a" \
  -DHB_HAVE_ICU=OFF \
  -DHB_HAVE_GLIB=OFF \
  -DHB_BUILD_TESTS=OFF

cmake --build build --config Release -j$JOBS
cmake --install build --config Release

echo "=== Android Dependencies build complete ==="
