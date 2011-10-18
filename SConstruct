# -*- mode: python -*-

import os

with open("VERSION", "r") as fin:
    package_version = fin.readline().strip()
    
Export("package_version")
SConscript('SConscript', variant_dir='build', duplicate=0)

# EOF #
