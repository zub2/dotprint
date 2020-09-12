#!/bin/sh
CONTAINER_NAME=dotprint
docker run --rm -v "$(pwd)":/work ${CONTAINER_NAME} $*