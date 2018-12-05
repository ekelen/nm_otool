#!/usr/bin/env zsh

for file in test/unit_test_files/fat/*
do
  	otool -t "$file" > ./out_otool/fat/"${file##*/}".out
done