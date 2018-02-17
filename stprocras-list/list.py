#!/usr/bin/env python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

import csv
from datetime import datetime, timedelta, timezone
    
# List tasks and time left for them

filename = "testfile"
firstcolumnname = "name"
secondcolumnname = "isodt"
columnnames = [firstcolumnname, secondcolumnname]

with open(filename, 'r', newline='') as csvfile:
    reader = csv.DictReader(csvfile, columnnames, delimiter='\t')
    for row in reader:
        dtwhen = datetime.strptime(row[secondcolumnname], "%Y-%m-%dT%H:%M:%S%z")
        utc_dtnow = datetime.now(timezone.utc)
        dtnow = utc_dtnow.astimezone()

        delta = dtwhen - dtnow

        if delta.days > 0:
            print(row['name'] + " - " + str(delta.days) + " days left")
        elif delta.days == 0:
            delta_sec = int(delta.total_seconds())

            delta_hour = delta_sec // 3600
            delta_sec -= delta_hour * 3600

            delta_min = delta_sec // 60
            delta_sec -= delta_min * 60

            print(row['name'] + " - " + str(delta_hour) + " hours, " + str(delta_min) + " minutes, " + str(delta_sec) + " seconds left")
        elif delta.days < 0:
            print(row['name'] + " - " + str(abs(delta.days)) + " days past")
