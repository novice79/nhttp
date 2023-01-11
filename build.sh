#!/bin/sh
# set -x

dir="_build"
[[ "$#" -gt 0 ]] && rm -rf $dir; rm -rf dist

# build http lib
cmake -GNinja -H"nhttp" -B"$dir/lib" \
-DCMAKE_FIND_ROOT_PATH="$HOME/clib-prebuilt/macos" \
-DCMAKE_INSTALL_PREFIX=dist \
-DCMAKE_BUILD_TYPE=Release 

cmake --build "$dir/lib"
cmake --install "$dir/lib"

# build example exe
cmake -GNinja -H"examples" -B$dir \
-DCMAKE_FIND_ROOT_PATH="$HOME/clib-prebuilt/macos;$PWD/dist" \
-DCMAKE_INSTALL_PREFIX=dist \
-DCMAKE_BUILD_TYPE=Release 

cmake --build $dir
cmake --install $dir

mkdir -p $PWD/dist/bin/{www,store}
echo "hello world from www dir static index.html file" > $PWD/dist/bin/www/index.html
echo "hello world from store dir static index.html file" > $PWD/dist/bin/store/index.html