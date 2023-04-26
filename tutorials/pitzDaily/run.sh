#!/usr/bin/bash

export FOAM_CODE_TEMPLATES=$PWD/../../etc/codeTemplates
cp -rT 0.orig 0
blockMesh
scalarTransportFoam
