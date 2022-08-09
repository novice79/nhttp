#!/bin/sh
# set -x

dir="_build"
[[ "$#" -gt 0 ]] && rm -rf $dir

# build http lib
cmake -GNinja -H"nhttp" -B"$dir/lib" \
-DCMAKE_FIND_ROOT_PATH="$HOME/clib-prebuilt/macos" \
-DCMAKE_INSTALL_PREFIX=dist \
-DCMAKE_BUILD_TYPE=Release 

cmake --build "$dir/lib"
cmake --install "$dir/lib"

# build example exe
cmake -GNinja -H. -B$dir \
-DCMAKE_FIND_ROOT_PATH="$HOME/clib-prebuilt/macos;$PWD/dist" \
-DCMAKE_INSTALL_PREFIX=dist \
-DCMAKE_BUILD_TYPE=Release 

cmake --build $dir
cmake --install $dir