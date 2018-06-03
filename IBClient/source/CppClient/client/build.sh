#!/usr/bin/env bash

mv /opt/xtnc/include/libibc /opt/xtnc/include/libibc_bak

cmake .

make

cp libibc.a /opt/xtnc/lib/libibc.a

cp -r ./includes/ /opt/xtnc/include/libibc/

rm libibc.a

rm Makefile