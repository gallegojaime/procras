#!/usr/bin/env python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

import csv
    
# List tasks and time left for them

filename = "testfile"
columnnames = ["name", "isodt"]
firstcolumnname = "name"
secondcolumnname = "iso-dt"

with open(filename, 'r', newline='') as csvfile:
    reader = csv.DictReader(csvfile, columnnames, delimiter='\t')
    for row in reader:
        print(row)
