#!/bin/sh
CONTAINER_NAME=dotprint
workdir=`pwd`
docker run --rm -v "${workdir}":/work ${CONTAINER_NAME} $*