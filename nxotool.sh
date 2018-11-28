#!/usr/bin/env bash

TESTFILE=$(find ./test -name \*${1}\* | tee /dev/tty)

FLAGS=${2:-" "}

myfunc()
{
	otool -t ${TESTFILE} ${FLAGS} 2>&1
}

myfunc