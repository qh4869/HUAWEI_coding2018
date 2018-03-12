#!/bin/bash

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
cd $BASEDIR

PACKAGE=_package
# valid files
bash validfile.sh
if [ $? -ne 0 ] 
then
    exit -1
fi

if [ -d ${PACKAGE} ]
then 
    rm -rf ${PACKAGE}
fi
cp -r origin-sdk-gcc/ ${PACKAGE}/
cp  ecs/*.cpp ecs/*.c ecs/*.h  ${PACKAGE}/ecs
cp origin-sdk-gcc/ecs/{CMakeLists.txt,io.cpp,ecs.cpp} ${PACKAGE}/ecs

cd ${PACKAGE}
bash build.sh
