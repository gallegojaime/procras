#!/usr/bin/env python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

import argparse, csv, logging, os, sys
from datetime import datetime, timedelta, timezone

def converttostring(listorstring):
    return ''.join(listorstring)
    
parser = argparse.ArgumentParser(description="Listing st-procras tasks included in a file.")

DEFAULTFILE = os.environ['HOME'] + "/.stprocras-events"

parser.add_argument("-f", "--file", default=DEFAULTFILE, help="Flat tab-separated values file to read from, overriding the default dotfile in home directory.")

parser.add_argument("-r", "--human", "--human-readable", action="store_true", help="Format output in a way more readable for humans, less friendly for input to other programs.")

args = parser.parse_args()

# FORMATSDICT will influence output format based on readability
FORMATSDICT = {
        'separator': "\t",
        'days': "d",
        'hours': "h",
        'minutes': "m",
        'seconds': "s",
        'machinepositive': "+",
        'machinenegative': "-",
        'humanleft': "",
        'humanpast': "",
        'humancomma': ""
        }

if args.human:
    FORMATSDICT = {
            'separator': " - ",
            'days': " days",
            'hours': " hours",
            'minutes': " minutes",
            'seconds': " seconds",
            'machinepositive': "",
            'machinenegative': "",
            'humanleft': " left",
            'humanpast': " past",
            'humancomma': ", "
            }

firstcolumnname = "name"
secondcolumnname = "isodt"
columnnames = [firstcolumnname, secondcolumnname]

try:
    with open(converttostring(args.file), 'r', newline='') as dsvfile:
        reader = csv.DictReader(dsvfile, columnnames, delimiter='\t')
        for row in reader:
            try:
                dtwhen = datetime.strptime(row[secondcolumnname], "%Y-%m-%dT%H:%M:%S%z")
            except (ValueError, TypeError) as e:
                logging.warning(sys.argv[0] + ": Date in line " + str(reader.line_num) + " does not adhere to format. Skipping.")
                continue

            utc_dtnow = datetime.now(timezone.utc)
            dtnow = utc_dtnow.astimezone()

            delta = dtwhen - dtnow

            # An "output" value to append things
            # line_num starts counting from 1, but we want to count from 0
            output = str(reader.line_num - 1) + FORMATSDICT['separator'] + row['name'] + FORMATSDICT['separator']

            if delta.days > 0:
                output += FORMATSDICT['machinepositive'] + str(delta.days) + FORMATSDICT['days'] + FORMATSDICT['humanleft']

            elif delta.days == 0:
                delta_sec = int(delta.total_seconds())

                delta_hour = delta_sec // 3600
                delta_sec -= delta_hour * 3600

                delta_min = delta_sec // 60
                delta_sec -= delta_min * 60

                output += (FORMATSDICT['machinepositive'] + str(delta_hour) + FORMATSDICT['hours'] + FORMATSDICT['humancomma'] +
                    str(delta_min) + FORMATSDICT['minutes'] + FORMATSDICT['humancomma'] +
                    str(delta_sec) + FORMATSDICT['seconds'] + FORMATSDICT['humanleft'])

            elif delta.days < 0:
                if args.human:
                    output += "\u001b[41;1m\u001b[37m" # Color alert

                output += FORMATSDICT['machinenegative'] + str(abs(delta.days)) + FORMATSDICT['days'] + FORMATSDICT['humanpast']

            if args.human:
                output += "\u001b[0m" # Reset any color change before next line

            print(output)

except FileNotFoundError as e:
    logging.critical(sys.argv[0] + " FileNotFoundError: default or provided database does not exist.")
    exit()
