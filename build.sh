#!/bin/bash

if [ ! -d disqt ]; then git submodule update --init; fi
if [ ! -d build ]; then mkdir build; fi
cd build
cmake ..
make -j$(nproc)
sudo make install