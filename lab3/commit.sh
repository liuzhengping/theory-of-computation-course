#!/bin/bash
tar zcvf utr-labos3.tar.gz *.cpp *.h
scp utr-labos3.tar.gz lo44313@pinus.cc.fer.hr:~/utr/utr-labos3.tar.gz
rm -f utr-labos3.tar.gz
