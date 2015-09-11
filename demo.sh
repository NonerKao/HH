#!/bin/bash

ROOTDIR=/home/scope
CLINC=-I/opt/intel/opencl-sdk/include/
CLLIB="-L/opt/intel/opencl-sdk/lib64/ -lOpenCL"
CLPRELOAD="LD_PRELOAD=/opt/intel/opencl-sdk/lib64/libOpenCL.so"

if [ $1"y" == "1y" ]; then
	cd $ROOTDIR/good_matvec
	gcc matvec.c $CLINC $CLLIB 2> /dev/null
	$CLPRELOAD ./a.out
elif [ $1"y" == "2y" ]; then
	cd $ROOTDIR/good_matvec
	../socket/client 192.168.0.102 $2
elif [ $1"y" == "3y" ]; then
	cd $ROOTDIR/bad_matvec
	../socket/client 192.168.0.102 $2
elif [ $1"y" == "4y" ]; then
	diff $ROOTFIR/good_matvec/matvec.c $ROOTDIR/bad_matvec/matvec.c
fi
