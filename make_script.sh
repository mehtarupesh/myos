#!/bin/bash

make_opt=${1:-}

make clean || exit 0
make $make_opt || exit 0
i586-elf-objcopy --only-keep-debug myos.bin myos.sym || exit 0
i586-elf-objcopy --strip-debug myos.bin