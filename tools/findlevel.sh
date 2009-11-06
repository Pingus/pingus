#!/bin/sh

find branches/pingus_0_6/data/levels/ -name "*.scm" | xargs grep -ie "$1"

