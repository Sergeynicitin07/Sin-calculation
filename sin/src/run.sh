#!/bin/bash

make clean >/dev/null 2>&1
make -j2 >/dev/null 2>&1

./hh_sim "$@"
