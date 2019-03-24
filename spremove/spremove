#!/usr/bin/env python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

import argparse, csv, logging, os, sys

def converttostring(listorstring):
    return ''.join(listorstring)

parser = argparse.ArgumentParser(description="Removing certain rows of a st-procras file, based on the input")

DEFAULTFILE = os.environ['HOME'] + "/.stprocras-events"

parser.add_argument("-f", "--file", default=DEFAULTFILE, help="Flat tab-separated values file to read from and write to, overriding the default dotfile in home directory.")
parser.add_argument('integers', metavar='rowNumber', type=int, nargs='+', help="Row numbers to be deleted.")

args = parser.parse_args()

firstcolumnname = "name"
secondcolumnname = "isodt"
columnnames = [firstcolumnname, secondcolumnname]

filename = converttostring(args.file)
tempfilename = filename + '~'

try:
    with open(filename, 'r+', newline='') as olddsvfile, open(tempfilename, 'w', newline='') as newdsvfile:
        reader = csv.DictReader(olddsvfile, columnnames, delimiter='\t')
        writer = csv.DictWriter(newdsvfile, fieldnames=columnnames, delimiter='\t')

        for row in reader:
            # The rows to delete will not be written into the new file
            # line_num starts counting from 1, so we subtract 1 from that number
            if ((reader.line_num - 1) not in args.integers):
                writer.writerow(row)

except FileNotFoundError as e:
    logging.critical(sys.argv[0] + " FileNotFoundError: default or provided database does not exist.")
    exit()

os.remove(filename)
os.rename(tempfilename, filename)
