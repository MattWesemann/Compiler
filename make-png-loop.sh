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
#	It needs to be provided a path to a folder containing a set of language files
#	for testing.
#
#	It will make a parsetree.png file in the 'root/output/language-file-name' directory,
#	as well as dumping the language-file-name.a, language-file-name.err, and 
#	language-file-name.p into the 'root/output/language-file-name' directory.
#
# 	TL;DR: Takes a directory, tests all files in the directory. Outputs files into:
#	'root/output/language-file-name'.


for file in ${1}*
do
	shortFile=${file##*/}
	cat ${file} | ./build/frontend ${file}
	if [ $? -ne 0 ] ; then
		echo -e "Processing ${file} resulted\nin an error. Check the\n./output/${shortFile} directory's .err file.\n"
	fi
	mkdir -p ./output/${shortFile}
	mv ./examples/toIrTests/${shortFile}.err ./output/${shortFile}
	mv ./examples/toIrTests/${shortFile}.p ./output/${shortFile}
	if [ -f ./examples/toIrTests/${shortFile}.a ] ; then
		mv ./examples/toIrTests/${shortFile}.a ./output/${shortFile}
	fi
	if [ -f ./examples/toIrTests/${shortFile}.ir ] ; then
		mv ./examples/toIrTests/${shortFile}.ir ./output/${shortFile}
	fi
	cd ./output/${shortFile}
	if [ -f ./examples/toIrTests/${shortFile}.a ]; then
		cat ${shortFile}.a | ../../parse-tree-to-graphvis.py | dot -T png -o parsetree.png
	fi
	cd ../..
done