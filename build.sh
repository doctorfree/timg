#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ../ -DWITH_POPPLER=Off -DWITH_VIDEO_DECODING=Off
make
