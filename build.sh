#!/bin/bash

# Builds the project using CMake

if ! [ -d build ]; then
    mkdir -v build
    cmake -S . -B build
fi

cmake --build build
