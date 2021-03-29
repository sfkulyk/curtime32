CURrent TIME'32 console utility

FREEWARE (C) 2004-2006 Sergey Kulik aka Saboteur

Contact author: saboteur@saboteur.com.ua

Purpose and usage:
Utility print all arguments to the standard output
Many years ago there were any tools for windows that could produce formatted date/time output like Unix/Linux "date".

Program continues CurTime tool (that was written for dos), so curtime32 works under win32.

There were a lot of issues disappears fixed in win32 version:
* No issues with long file names
* No command line lenght limitation
* Only win32 api uses, so no dos-machine routines

See curtime32 /help to find all substutution combination.
* You can use $+XX and $-XX changing output date, also you can use timestamp to set output date.
* There are english and russian names for weekdays and months.
* Formatted output could be executed with "/exec" command.

You can download source package with compiled binaries for curtime32 and 16-bit curtime.

Detailed help: https://github.com/sfkulyk/curtime32/wiki
               curtime32 /help
               
Examples:
$ curtime32 "$y/$M/$d process started"
21/03/29 process started

$ curtime32 "Today is $w and tomorrow $+24h will be $W"
Today is Mon and tomorrow  will be Tuesday

$ curtime32 "zip a $y$M$d.zip file.log" /exec
Executing: [zip a 210329.zip file.log]
....

$ echo "$h-$m-$s" | curtime32
"16-42-07"
