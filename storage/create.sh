#!/bin/bash

set -xe

rm -rf vol
rm -rf out
python2 storage/dsplit.py -s 50 assets/ vol
mkdir out
ls vol | while read line
do
    pushd vol/$line
    $EMSDK/upstream/emscripten/tools/file_packager $line.data --preload assets --js-output=$line.js --use-preload-cache --no-node
    mv $line.data ../../out/
    mv $line.js ../../out/
popd
done
rm -rf vol
cp build/GrafikaKomputerowa.js out/
cp build/GrafikaKomputerowa.wasm out/
cp build/index.html out/
