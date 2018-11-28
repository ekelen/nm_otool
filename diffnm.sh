#!/usr/bin/env bash

TESTFILE=$(find ./test -name \*${1}\* | tee /dev/tty)

FLAGS=${2:-" "}

myfunc()
{
	diff <(./ft_nm ${TESTFILE} ${FLAGS}) <(nm ${TESTFILE} ${FLAGS})
}

myfunc