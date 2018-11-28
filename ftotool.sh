#!/usr/bin/env bash

TESTFILE=$(find ./test -name \*${1}\* | tee /dev/tty)

FLAGS=${2:-" "}

myfunc()
{
	local var=$(./ft_otool ${TESTFILE} ${FLAGS} )
	echo "$var"
}

myfunc