#!/usr/bin/python
import sys

path = sys.argv[1] if len(sys.argv) > 1 else "out/test"
with open(path, "w") as f:
    for i in "abcdefghijklmnopqrstuvwxyz":
        f.write("{}{}\n".format(i, i.capitalize()))
