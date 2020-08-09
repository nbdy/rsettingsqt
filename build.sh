#!/bin/bash

if [ ! -d disqt ]; then git submodule update --init; cd disqt; ./build.sh; fi
if [ ! -d build ]; then mkdir build; fi
cd build
cmake ..
make -j$(nproc)
sudo make install