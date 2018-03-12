#!/bin/bash

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
cd $BASEDIR

diff ecs/ecs.cpp origin-sdk-gcc/ecs/ecs.cpp
tmp=$?
if [ ${tmp} -ne 0 ]
then
    echo "$BASEDIR/ecs/ecs.cpp was changed!!!"
    exit -1
fi
diff ecs/io.cpp origin-sdk-gcc/ecs/io.cpp
tmp=$?
if [ ${tmp} -ne 0 ]
then
    echo "$BASEDIR/ecs/io.cpp was changed!!!"
    exit -1
fi

exit 0

