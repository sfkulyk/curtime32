#English help

#Russian help page

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
> CURrent TIME'32 console utility
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
1. License:
> CURTIME32 - Current Time'32 console utility
> FREEWARE (C) 2004-2005 Sergey Kulik aka Saboteur
> Contact author: saboteur@saboteur.com.ua

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
2.
> The program simply gives all the arguments to standard output, but in addition allows the use of macros, which are replaced by appropriate values (See help for macros).
> Utility was created to ease the writing of BAT-script, accurate records console logs and other usage.
> The program is a continuation of the project CurTime, translated in win32 application.
> Due to port automatically disappeared following problems that were DOS-version curtime:
    * Now there are no problems with long file names
    * Now there is no limit command line length of 128 characters
    * Used only api functions, therefore, is not called dos-machine       and program execution is faster and more correctly under win32 systems
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
3. Usage examples:

a) Write execution date/time to the file. Filename contains current day and month.

> --- begin of backup.bat ---
> curtime32 "Process started $T" /file d:\backup\$d$M.log
> rar a -r d:\backup\periodic.rar c:\base\**.**
> curtime32 "Process finished $T$c==========" /file d:\backup\$d$M.log
> ---- end of backup.bat ---

b) Execute archivation of c:\bases to the archive, name of the archive contain year, month and day:

> --- begin of backup.bat ---
> curtime32 "rar.exe a -r d:\Backup_$y$M$d.rar c:\bases\**.**" /exec >>backup.log
> ---- end of backup.bat ---_

d) Support for input redirection:

> echo Today is $V|curtime32
> type text.txt|curtime32

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
4. Full help:

> Usage:
> curtime32.exe
> > When you run the program without arguments, it will wait for input from standard input STDIN. redirection is supported.


> curtime32.exe arg1 arg2 arg3... argn [> >> output]
> > output - can be a file or other stream, if not specified - to the screen. Arguments can be enclosed in quotes, the arguments without the quotes together, separated from each other with a space that is
> > curtime32 arg1 arg2     = "arg1 arg2"
> > curtime32 arg1   arg2   = "arg1 arg2"
> > curtime32 "arg1   arg2" = "arg1   arg2"


> You can redirect to curtime from another program, such as echo
> > echo arg | curtime32.exe

# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

> Help for arguments:

> -h - full macro help
> /file 

&lt;filename&gt;

 - Set filename for output. Can contain macros.
> /exec            - happen execution line arguments, which is very
> > useful for automating processes

> /time            - used only in conjunction with the / exec, otherwise
> > produces no action. Upon the completion of
> > command displays the duration of its execution.


> Macroses:

> $c - transfer line
> $p - space
> \" - character quotes
> $$ - dollar sign
> $T - time in format     [hh:mm:ss]
> $D - date in format     [dd yyyy](Jan.md)
> $Z - date in format     [Янв дд гггг]
> $F - date/time in format[Jan dd hh:mm:ss yyyy](Mon.md)
> $f - date/time in format[Пн Янв дд чч:мм:сс гггг]
> $Y - year (four-number) [yyyy](yyyy.md)
> $y - year (two-number)  [yy](yy.md)
> $j - month in format    [Jan](Jan.md)
> $J - month in format    [January](January.md)
> $g - month in format    [Янв]
> $g - month in format    [Январь]
> $M - month in format    [xx](xx.md)
> $d - day of the month   [xx](xx.md)
> $n - day of the week    [xx](xx.md)
> $w - day of the week    [Mon](Mon.md)
> $W - day of the week    [Monday](Monday.md)
> $v - day of the week(rus)[Пн]
> $V - day of the week(rus)[Понедельник]
> $h - hours               [xx](xx.md)
> $m - minutes             [xx](xx.md)
> $s - seconds             [xx](xx.md)
> $S - mseconds            [xxx](xxx.md)
> $U - seconds from 1.1.1970 (unix-like) [xxxxxxxxxx](xxxxxxxxxx.md)
> $u - Converting from unix-time.
> > Syntax:: after $ u should go to 10-digit time in Unix format
> > (Number of seconds since 1970). If more digits - other perceived as plain text.
> > If it is less - the first did not figure indicates the end of the data.
> > After the command $ u time it is possible to deduce any other teams -
> > it's set as if it were established with $ or $ -.
> > Function is not fully verified. Such as the time will not be shown
> > Regional (eg 2 hours), and Greenwich.
> > Examples:
> > > > Curtime32 "$ u0 $ Y $ M $ d $ h: $ m: $ s"
      1. 70 01 01 00:00:00


> >curtime32.exe $u113025982$w $j $d $h:$m:$s $Y
> Wed Aug 01 04:06:22 1973

> $+xy - shift date/time (add)
> $-xy - shift date/time (substract)
> > x - from 0 to 32000
> > y - s-seconds, m-minutes, h-hours, d-days
if x = 1, then it can not write ($ 1d, too, that $ d)
> > The last three commands do not display information, but only change,
> > that is, if you want to show how many will be tomorrow, it is necessary
> > write "$ d $ d" - $ d - we add one day, $ d - derive a date.
> > "$ 2d $ d $-5d $ d $ **$ d"
> > add two days to withdraw the number, subtract 5 days to derive a number
> > set the current time / date display number.

> $** - Change the output date / time for the current (reset changes, update)
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #
5. History:
Version
> v 1.0 - first release after porting to dos16 in win32
> > added milliseconds
> > - Found problems with encodings
> > - Temporarily not working "shift" of time
> > Thanks for bug reports Goncharov.A.M.

> v 1.2 - Fixed bug with $ $
> > - Fixed bug with numbers of months

> v 1.3 - Fixed bug with numbered days of the week
> > - Fixed bug with encoding when forwarding echo | curtime32

> v 1.5 - restored work "time shift"
> > Thanks for bug reports Pavel A Resh

> v 1.6 - added help-file
> > - Fixed ochepyatki in the code

> v 1.7 - 15.08.2005 Added the ability to convert the time of the unix-like in any
> v 1.8 - 12.08.2008 Fixed type constructions curtime32 "echo aa" / exec / file file.txt
> > in which the conclusion was in aa std and not in the specified file.txt

> v 1.9 - 18.09.2008 added $ U - displaying the number of seconds since 1/1/1970, the (unix-format)
> v 1.10-27.04.2009 fixed work with the coding, now correctly displays
> > Russian characters in the command line in batch file (in bat files should be
> > use DOS encoding CP866)
> > Thanks for bug reports Yakov Kaluzhenko
# -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #