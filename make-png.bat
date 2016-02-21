@echo off
type examples\testlang | build\Debug\frontend.exe outputsimple
type outputsimple.p | python parse-tree-to-graphvis.py | dot -Tpng -o parsetree.png