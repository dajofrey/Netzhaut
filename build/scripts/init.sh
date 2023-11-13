#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd ${SCRIPT_DIR}/../../external

# Download Vulkan headers.
wget -O tmp.zip https://github.com/KhronosGroup/Vulkan-Headers/archive/v1.3.220.zip
unzip -o tmp.zip

# Download volk, a Vulkan meta loader.
wget -O tmp.zip https://github.com/zeux/volk/archive/master.zip
unzip -o tmp.zip

# Download OpenGL meta loader. Currently disabled because we made changes to it that are not in upstream.
wget -O tmp.zip https://github.com/tsherif/simple-opengl-loader/archive/master.zip
unzip -o tmp.zip

# Clean downloads.
rm tmp.zip

# Copy Vulkan related includes.
cp volk-master/volk.h ../src/lib/nhgfx/Common/Vulkan
cp Vulkan-Headers-1.3.220/include/vulkan/* ../src/lib/nhgfx/Common/Vulkan

# Copy OpenGL related includes.
cp simple-opengl-loader-master/simple-opengl-loader.h ../src/lib/nhgfx/Common/OpenGL
cp glx/glx.h ../src/lib/nhgfx/Common/OpenGL
cp glx/glxext.h ../src/lib/nhgfx/Common/OpenGL

# Compile xxd.
cd xxd
gcc -o xxd xxd.c

# Compile freetype-gl.
cd ../freetype-gl
gcc -fPIC -I/usr/include/freetype2 -c -o utf8-utils.o utf8-utils.c -lfreetype
gcc -fPIC -I/usr/include/freetype2 -c -o vector.o vector.c -lfreetype
gcc -fPIC -I/usr/include/freetype2 -c -o texture-atlas.o texture-atlas.c -lfreetype
gcc -fPIC -I/usr/include/freetype2 -c -o texture-font.o texture-font.c -I/usr/include/harfbuzz -lfreetype -lharfbuzz
gcc -fPIC -I/usr/include/freetype2 -c -o distance-field.o distance-field.c -lfreetype
gcc -fPIC -I/usr/include/freetype2 -c -o edtaa3func.o edtaa3func.c -lfreetype
gcc -fPIC -I/usr/include/freetype2 -c -o platform.o platform.c -lfreetype
gcc -shared utf8-utils.o vector.o texture-atlas.o texture-font.o distance-field.o edtaa3func.o platform.o -I/usr/include/freetype2 -lfreetype -lharfbuzz -o libfreetype-gl.so

# Compile C hashmap.
cd ../c_hashmap
gcc -c -o hashmap.o hashmap.c
ar rcs libc_hashmap.a hashmap.o

#  Compile volk.
cd ../volk-master
gcc -c -o volk.o volk.c -ldl -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface -DVOLK_VULKAN_H_PATH=\"../Vulkan-Headers-1.3.220/include/vulkan/vulkan.h\"
ar rcs libvolk.a volk.o

# Compile st-0.8.5.
cd ../st-0.8.5
gcc -fPIC -c -o st.o st.c
gcc -shared st.o -o libst.so
