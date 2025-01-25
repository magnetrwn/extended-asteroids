#!/usr/bin/bash

set -e

mkdir -p build
rsync -a static/ build/

cd build
cmake ..
make -j4

mv compile_commands.json ../

rm asteroids.zip || true
zip -r9 asteroids.zip asteroids asteroids.ini res

./asteroids
cd ..
