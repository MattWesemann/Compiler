#!/bin/bash

#   Assumes the following folder setup:
#
#   The 'root' directory is the git-repo root, 'Compiler'
#
#   A 'frontend' executable has already been built (see README.md)
#   in the root.
#	
#   This script should be executed from 'root'.
#	
#	It needs to be provided a language file ${1} and a output file ${2}.
#
#	It will make a parsetree.png file in the 'root' directory, as well as dumping
#	the ${2}.a, ${2}.err, and ${2}.p into the 'root' directory.

cat ${1} | ./build/frontend ${2}
cat ${2}.a | ./parse-tree-to-graphvis.py | dot -T png -o parsetree.png