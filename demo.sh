#!/bin/bash

if [ $1"y" == "1y" ]; then
	cd good_matvec
	gcc matvec.c -I/opt/intel/opencl-sdk/include/ -L/opt/intel/opencl-sdk/lib64/ -lOpenCL 2> /dev/null
	LD_PRELOAD=/opt/intel/opencl-sdk/lib64/libOpenCL.so ./a.out
elif [ $1"y" == "2y" ]; then
	cd good_matvec
	../socket/client 192.168.0.102 $2
elif [ $1"y" == "3y" ]; then
	cd bad_matvec
	../socket/client 192.168.0.102 $2
elif [ $1"y" == "4y" ]; then
	diff good_matvec/matvec.c bad_matvec/matvec.c
fi
