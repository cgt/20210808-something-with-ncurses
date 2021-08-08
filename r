#!/bin/sh

make && git commit --quiet --all -m "It compiles" && ./main
