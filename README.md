                       8
              .oPYo.  o8P       .oPYo. oPYo. .oPYo. .oPYo. oPYo. .oPYo. .oPYo.
              Yb..     8  ooooo 8    8 8  `' 8    8 8    ' 8  `' .oooo8 Yb..
                'Yb.   8        8    8 8     8    8 8    . 8     8    8   'Yb.
              `YooP'   8        8YooP' 8     `YooP' `YooP' 8     `YooP8 `YooP'
              :.....:::..:::::::8 ....:..:::::.....::.....:..:::::.....::.....:
              ::::::::::::::::::8 :::::::::::::::::::::::::::::::::::::::::::::
              ::::::::::::::::::..:::::::::::::::::::::::::::::::::::::::::::::
st-procras: keep your deadlines in check.

The st-procras package improves your, or your team member's, situational awareness. Keeping a local database of deadlines, one can use it alone or as the base for
an organization's deadline management system.

# Prerequisites
Python 3 and dateparser, pytz, tzlocal.

# Usage
This CLI version of st-procras is pretty simple to use. The fundamental operations it encompasses
are the listing, adding, modification and removal of deadlines.

## Adding deadlines
The `spadd` command will let us add a deadline. The syntax is `spadd [-f FILE] title [time]`,
where:
* `title` is a (usually quoted) string denoting the title of the deadlined task.
* `time` is an optional parameter which accepts absolute (ISO formats, 12 and 24-hour times) and relative
time formats. Natural language, such as `tomorrow`, `October 23`, or `in 2 weeks`, is supported.
* the flag `-f` signals the input of an alternate flat file (path `FILE`) in which to store the deadline.

More detailed information is reachable by calling `spadd -h` or `spadd --help`.

## Listing deadlines
The `splist` command shows all stored deadlines and time left to reach them. The syntax is `splist [-f FILE]`.

If you don't use `splist` for machine-friendly output, the `-r` (or `--human`, `--human-readable`) flag will format a prettier output, and past deadlines will be signaled by a dramatic color change. 

More detailed information is reachable by calling `splist -h` or `splist --help`.

## Modifying and removing deadlines
Deadline modification and removal are currently a work-in-progress, but the workaround is not complicated.
Simply open the flat file containing the deadline with your favourite text editor (Notepad, vim, emacs...;
document editors such as Word are discouraged). Find the line containing the deadline, which will consist
of the title and the UTC time separated by a tab space. Modify these at your liking or delete the whole line.

# Default flat file
The "flat file" which st-procras uses as a database is the dotfile `.stprocras-events` in the `$HOME` directory.
The `-f` flag in the st-procras commands overrides default behavior and allows for an alternate file to operate with.

## Internal time format for the database
The time format used internally is ISO 8601 compliant UTC time: `YYYY-MM-DDTHH:MM:SS+ZZZZ`. *This is the only permissible format
and must be exact.*
The +ZZZZ offset will be +0000 by default, signifying null deviation from UTC time.
If written manually into the database, nonzero offsets are supported for the programmer's comfort, although not encouraged.

# TODOs
Change machine-friendly time output to "-1d" for 1 day past, "2w" for in 2 weeks, etc.

# Authors
* Jaime `tri0d` Gallego, initial work, MW version.

# License
The `st-procras` program compendium is protected under the GNU GPL v3 license (must be the libertarian in me). For the full version, read the LICENSE file.
