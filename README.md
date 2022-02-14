CURrent TIME'32 console utility<br />
FREEWARE (C) 2004-2006 Sergey Kulik aka Saboteur<br />
Contact author: saboteur@saboteur.com.ua<br />
<br />
Purpose and usage: <br />
Utility print all arguments to the standard output<br />
Many years ago there were no suitable ools for windows that could produce formatted date/time output like Unix/Linux "date".<br />
So it is my alternative with some useful features. Should work in any win32 system.<br />

This program was based on my curtime (dos-16), and the following major features were fixed<br />
* No issues with long file names<br />
* No command line lenght limitation<br />
* Only win32 api uses, so no dos-machine routines<br />

See curtime32 /help to find all substututions.<br />
* You can use $+XX and $-XX to set relative utput date, also you can use timestamp<br />
* There are english and russian names for weekdays and months.<br />
* Formatted output could be evaluated (executed) with "/exec" option<br />
<br />
You can download source package with compiled binaries for curtime32 and 16-bit curtime.<br />
<br />
Detailed help: https://github.com/sfkulyk/curtime32/wiki<br />
               curtime32 /help<br />
<br />
Examples:<br />
$ curtime32 "$y/$M/$d process started" <br />
21/03/29 process started<br />
<br />
$ curtime32 "Today is $w and tomorrow $+24h will be $W" <br />
Today is Mon and tomorrow  will be Tuesday<br />
<br />
$ curtime32 "zip a $y$M$d.zip file.log" /exec <br />
Executing: [zip a 210329.zip file.log]<br />
....<br />
<br />
$ echo "$h-$m-$s" | curtime32 <br />
"16-42-07"<br />
