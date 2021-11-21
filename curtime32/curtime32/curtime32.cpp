#include <afxwin.h>         // MFC core and standard components
// #include <winbase.h>

#define TRUE  1
#define FALSE 0

/* Curtime32 (C) Sergey Kulik aka Saboteur
  
   v 1.0 - first worked ported version 
         - adjusting work bad
         + milliseconds
         + troubles with russian letters
   v 1.2 - fixed bug with $$
         - fixed bug with month number
   v 1.3 - fixed bug with weekday numeration
         - fixed bug with characters by echo |curtime32
   v 1.5 - restored functionality with add/sub time and dates
   v 1.6 - help file added, fixed some spellings
   v 1.7 - 15.08.2005 unix-time conversion added
   v 1.8 - 12.08.2008 fixed constructions like curtime32 "echo >> a.txt" /exec /file a.txt
   v 1.9 - 18.09.2008 added $U - echo unix-like time in seconds from 1970
   v 1.10 - 27.04.2009 fixed russian CP866 codepage
*/

/* Structure declaration */
struct curtime_data // for table
{
  int	number;		// number in table
  char	tnumber[3];	// weekday [dd], Sunday = 1
  char	rnumber[3];	// weekday [dd], Monday = 1 for reversing
  char	month[4];	// Jan
  char	rmonth[4];	// Янв
  char	fmonth[10];	// month [January]
  char	rfmonth[10];// month [Январь]
  char	wday[4];	// weekday [Mon]
  char	rday[4];	// weekday [Пн]
  char	fday[12];	// weekday [Monday]
  char	rfday[12];	// weekday [Понедельник]
  int	days;		// days in month (feb = 28) - not used
};

const struct curtime_data table[] =
{

{  1, "01", "07", "ERR", "ERR", "Error",     "Error",    "Sun", "Вс",  "Sunday",    "Воскресенье", 00 },
{  2, "02", "01", "Jan", "Янв", "January",   "Январь",   "Mon", "По",  "Monday",    "Понедельник", 31 },
{  3, "03", "02", "Feb", "Фев", "February",  "Февраль",  "Tue", "Вт",  "Tuesday",   "Вторник",     28 },
{  4, "04", "03", "Mar", "Мар", "March",     "Март",     "Wed", "Ср",  "Wednesday", "Среда",       31 },
{  5, "05", "04", "Apr", "Апр", "April",     "Апрель",   "Thu", "Чт",  "Thurthday", "Четверг",     30 },
{  6, "06", "05", "May", "Май", "May",       "Май",      "Fri", "Пт",  "Friday",    "Пятница",     31 },
{  7, "07", "06", "Jun", "Июн", "June",      "Июнь",     "Sat", "Сб",  "Saturday",  "Суббота",     30 },
{  8, "08", "00", "Jul", "Июл", "July",      "Июль",     "Sun", "Вс",  "Sunday",    "Воскресенье", 31 },
{  9, "09", "00", "Aug", "Авг", "August",    "Август",   "err", "err", "error",     "ошибка",      31 },
{ 10, "10", "00", "Sep", "Сен", "September", "Сентябрь", "err", "err", "error",     "ошибка",      30 },
{ 11, "11", "00", "Oct", "Окт", "October",   "Октябрь",  "err", "err", "error",     "ошибка",      31 },
{ 12, "12", "00", "Nov", "Ноя", "November",  "Ноябрь",   "err", "err", "error",     "ошибка",      30 },
{  13, "13", "00", "Dec", "Дек", "December",  "Декабрь", "err", "err", "error",     "ошибка",      31 }
};                           

/* local defines */
#undef isspace
#define isspace(x)    (x==' ' || x=='\t' || (x > 9 && x < 14) )
#define IS_DIGIT(c)   ((c)>=48 && (c)<=57)

#define IS_SET(flag, bit)    ((flag) & (bit))
#define SET_BIT(var, bit)    ((var) |= (bit))
#define REM_BIT(var, bit)    ((var) &= ~(bit))
#define FOUT                 IS_SET(flag,OUT_TO_FILE)?fp:stdout

#define OUT_TO_FILE 1
#define STD_IN      2
#define EXECUTE     4
#define SHOW_TIME   8

/* local function declaration */
int str_prefix( const char *astr, const char *bstr );
int str_cmp( const char *astr, const char *bstr );
void adjust_date(unsigned __int64 changer, bool change, bool fromnull); // for ajusting time via changer
int interpret(char *buf,char *output);			   // interpreting string

/* global variables */
int		flag;
SYSTEMTIME curtime;

int main(int argc, char **argv)
{
 FILE	*fp;
 TCHAR	filename[8192], output[8192], buf[8192], oemtmp[8192], fileout[8192];
 unsigned int i;
 int	echospace;

 if ( !str_prefix(argv[1],"/help") || !str_prefix(argv[1],"/HELP")
   || !str_cmp(argv[1],"-h") || !str_cmp(argv[1],"-H")
   || !str_cmp(argv[1],"-help") || !str_cmp(argv[1],"--help")
   || !str_cmp(argv[1],"/?") || !str_cmp(argv[1],"-?"))
 {
    _stprintf(buf,
    " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
    " = Curtime32 v1.10       (C) 27.04.2009  Saboteur aka Sergey Kulik =\n"
    " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n"
    " curtime [\"args\"] [/exec] [/time] [/file <outfile>] [< in] [> >> out]\n"
    " ==--==--==--==--==--==-- Macros list --==--==--==--==-=-=-=--==--==\n"
    "  $D - date [dd Jan yyyy]  $Z - дата [дд Янв гггг]\n"
    "  $j - month [Jan]         $g - месяц [Янв]\n"
    "  $J - month [January]     $G - месяц [Январь]\n"
    "  $w - weekday [Sun]       $v - день недели [Ср]\n"
    "  $W - weekday [Sunday]    $V - день недели [Среда]\n"
    "  $n - weekday [dd] sun=1  $r - weekday [dd] mon=1\n\n"
    "  $h - hours   [hh]        $Y - year [yyyy]         \n"
    " $m - minutes [mm]        $y - year [yy]           \n"
    "  $s - seconds [ss]        $M - month [mm]          \n"
    "  $S - milliseconds [SSS]  $d - day [dd]            \n\n"
    "  $T - full time format [hh:mm:ss]\n"
	"  $U - unix time [xxxxxxxxxx] (seconds from 1.1.1970)\n\n"
    "  \\\" - symbol [\"]          $c - cr/lf\n"
    "  $p - space [ ]           $+xy or $-xy - increase/decrease (y=dhms)\n"
    "  $$ - symbol [$]          $*   - restore date&time\n"
	"                           $uxxxxxxxxxx - set time from unixformat\n\n"
    "  Example: c:>curtime32 Today is $W, $T, and tomorrow will be $+d$W.\n"
    "           Today is Monday, 12:00:00, and tomorrow will be Tuesday.\n\n"
    "           c:>curtime32 \"ren today.log $y$M$d_log.txt\" /exec\n"
    "           1 file(s) moved\n"
    " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
	_ftprintf(stdout,buf);
    return (0);                                     
 }

 GetLocalTime( &curtime );
 buf[0]='\0';
 echospace=0;

 if (argc<2)
 {
   int i;
   char a;
   
   SET_BIT(flag,STD_IN);
   for(i=0;;i++)
   {
     if feof(stdin) break;
     a=_fgetchar();
     buf[i]=a;
   }
   buf[i]='\0';
 }
 if (!IS_SET(flag,STD_IN))
 {
   for (i=1;argv[i]!='\0';i++)
   {
     if (!str_prefix(argv[i],"/exec") || !str_prefix(argv[i],"--exec"))
     {
       SET_BIT(flag,EXECUTE);
       continue;
     }
     if (!str_prefix(argv[i],"/time") || !str_prefix(argv[i],"--time"))
     {
       SET_BIT(flag,SHOW_TIME);
       continue;
     }

     if (!str_prefix(argv[i],"/file") || !str_prefix(argv[i],"/FILE") || !str_prefix(argv[i],"--file"))
     {
       _stprintf (filename,argv[i]+6);
       SET_BIT(flag,OUT_TO_FILE);
       i++;
       continue;
     }
     if (echospace==1) _tcscat(buf," ");
     echospace=1;
     _tcscat(buf,argv[i]);
   }
 }

 output[0]='\0';
 if (!IS_SET(flag,EXECUTE))
 {
	 CharToOemA(buf,oemtmp);
	 interpret(oemtmp,output);
 }
 else
 {
	 interpret(buf,output);
 }
 
 if (IS_SET(flag,OUT_TO_FILE))
 {
   _stprintf(buf,"");
   interpret(filename,fileout);
   if ((fp=fopen(fileout,"a"))==NULL)
   {
     _ftprintf(stdout,"\n - Error - Can't open file %s\n",fileout);
     return 0;
   }
   fseek(fp,0,SEEK_END);
 }

 if (IS_SET(flag,EXECUTE))
 {
   TCHAR	cmd[8192];
   TCHAR	oemtmp[8192];
// CharToOemA - for correct /exec command output
   CharToOemA(output,oemtmp);
   _ftprintf(FOUT,"Executing: [%s]\n",oemtmp);
   if (IS_SET(flag,SHOW_TIME))
   {
     SYSTEMTIME newtime;
     
     GetLocalTime( &newtime );
     _ftprintf(FOUT,"Begin: %02d:%02d:%02d.%03d\nFinish: %02d:%02d:%02d.%03d",
		 curtime.wHour,curtime.wMinute,curtime.wSecond,curtime.wMilliseconds,
		 newtime.wHour,newtime.wMinute,newtime.wSecond,newtime.wMilliseconds);
   }
   else
   {
	 if (IS_SET(flag,OUT_TO_FILE))
	 {
        fclose (fp);
		_stprintf(cmd,"%s >> %s",output,fileout);
	    system(cmd);
	 }
	 else
	 {
		 system (output);
	 }
   }
   return 0;
 }
 _ftprintf(FOUT,output);
 return 0;
}

void adjust_date(unsigned __int64 changer, bool chang,bool fromnull)
{
  FILETIME ftime;
  LPFILETIME chtime=&ftime;
  ULARGE_INTEGER diff, temp;

  diff.QuadPart=(unsigned __int64) changer;
  diff.QuadPart*=10000000;
  
/* BOOL FileTimeToSystemTime( const FILETIME* lpFileTime, LPSYSTEMTIME lpSystemTime );
   BOOL SystemTimeToFileTime( const SYSTEMTIME* lpSystemTime, LPFILETIME lpFileTime );
   case 's' sec     1 sec = dwLowDateTime  10.000.000
   case 'm' min    60 sec = dwLowDateTime 600.000.000
   case 'h' hour 3600 sec = dwHighDateTime = 8, dwLowDateTime = 1600.000.000
   case 'd' day 86400 sec = dwHighDateTime = 200, dwLowDateTime = 4000.000.000
   #define  ULONGMAX 4294967296
*/
    if (fromnull)
	{
      temp.QuadPart=116444736000000000;   // seconds from 1.01.1970 00:00:00
//	  temp.QuadPart=72000000000;   // + 2 hours - region zone
	  temp.QuadPart+=changer*10000000;    // + changer seconds
	  
	  ftime.dwHighDateTime=temp.HighPart;
	  ftime.dwLowDateTime=temp.LowPart;
	}
	else
	{
      SystemTimeToFileTime( &curtime, chtime);
      temp.HighPart=ftime.dwHighDateTime;
	  temp.LowPart=ftime.dwLowDateTime;
	  if (chang) temp.QuadPart+=diff.QuadPart;
	  else temp.QuadPart-=diff.QuadPart;
	  ftime.dwHighDateTime=temp.HighPart;
	  ftime.dwLowDateTime=temp.LowPart;
	}
	FileTimeToSystemTime ( chtime, &curtime);
}

int str_prefix( const char *astr, const char *bstr )
{
  if ( astr == NULL ) return TRUE; /* BUG */
  if ( bstr == NULL ) return TRUE; /* BUG */

  while (*astr) { if (*astr++ != *bstr++) return TRUE;}

//  for ( ; *astr; astr++, bstr++ )
//  {
//    if (*astr != *bstr) return TRUE;
//  }
  return FALSE;
}

int str_cmp( const char *astr, const char *bstr )
{
  if ( astr == NULL ) return TRUE;
  if ( bstr == NULL ) return TRUE;

  while (*astr||*bstr) { if (*astr++ != *bstr++) return TRUE; }

//  for ( ; *astr || *bstr; astr++, bstr++ )
//  {
//    if ( *astr != *bstr ) return TRUE;
//  }
  return FALSE;
}

int interpret(char *buf,char *output)
{
 int i;
 TCHAR temp[8192];

 for (i=0;buf[i]!='\0';i++)
 {
   if (buf[i]=='$')
   {
	 i++;
     switch (buf[i])
     {
	   case '\0':_stprintf(temp,"$"); break;
	   case '$': _stprintf(temp,"$"); break;
 	   case 'D': _stprintf(temp,"%02d %s %04d",curtime.wDay,table[curtime.wMonth].month,curtime.wYear); break;
       case 'F': _stprintf(temp,"%s %s %02d %02d:%02d:%02d %04d", table[curtime.wDayOfWeek].wday,table[curtime.wMonth].month,curtime.wDay,curtime.wHour,curtime.wMinute,curtime.wSecond,curtime.wYear); break;
       case 'G': _stprintf(temp,"%s",table[curtime.wMonth].rfmonth);     break;
       case 'J': _stprintf(temp,"%s",table[curtime.wMonth].fmonth);     break;
       case 'M': _stprintf(temp,"%02d",curtime.wMonth);    break;
       case 'T': _stprintf(temp,"%02d:%02d:%02d",curtime.wHour,curtime.wMinute,curtime.wSecond); break;
	   case 'U': 
		   {
			LARGE_INTEGER utcFT = {0};
			SystemTimeToFileTime(&curtime, (FILETIME*)&utcFT);
			time_t dosTime = (time_t) (utcFT.QuadPart/10000000 - 11644484400);
			_stprintf(temp,"%ld",dosTime);
			break;
		   }
       case 'V': _stprintf(temp,"%s",table[curtime.wDayOfWeek].rfday);      break;
       case 'W': _stprintf(temp,"%s",table[curtime.wDayOfWeek].fday);      break;
       case 'Y': _stprintf(temp,"%04d",curtime.wYear); break;
       case 'Z': _stprintf(temp,"%s %02d %04d",table[curtime.wMonth].rmonth,curtime.wDay,curtime.wYear);break;
       case 'c': _stprintf(temp,"\n"); break;
       case 'd': _stprintf(temp,"%02d",curtime.wDay);                break;
       case 'f': _stprintf(temp,"%s %s %02d %02d:%02d:%02d %04d",table[curtime.wDayOfWeek].rday,table[curtime.wMonth].rmonth,curtime.wDay,curtime.wHour,curtime.wMinute,curtime.wSecond,curtime.wYear); break;
       case 'g': _stprintf(temp,"%s",table[curtime.wMonth].rmonth);      break;
       case 'h': _stprintf(temp,"%02d",curtime.wHour); break;
       case 'j': _stprintf(temp,"%s",table[curtime.wMonth].month);      break;
       case 'm': _stprintf(temp,"%02d",curtime.wMinute);  break;
       case 'n': _stprintf(temp,"%s",table[curtime.wDayOfWeek].tnumber);   break;
       case 'p': _stprintf(temp," ");  break;
       case 'r': _stprintf(temp,"%s",table[curtime.wDayOfWeek].rnumber);   break;
       case 's': _stprintf(temp,"%02d",curtime.wSecond);  break;
       case 'S': _stprintf(temp,"%03d",curtime.wMilliseconds);  break;
       case 'u':
	   {
         unsigned __int64 uchanger=0;
		 int ucounter=0;

		 while (++ucounter<=10)
		 {
			if ((int)buf[++i]<48 || (int)buf[i]>57){ i--;break;}
			uchanger*=10;
			uchanger+=(buf[i])-48;
 		 }
		 adjust_date(uchanger,1,1);
		 break;
	   }
       case 'v': _stprintf(temp,"%s",table[curtime.wDayOfWeek].rday);       break;
       case 'w': _stprintf(temp,"%s",table[curtime.wDayOfWeek].wday);       break;
       case 'y': _stprintf(temp,"%02d",curtime.wYear-(curtime.wYear/100*100)); break;
       case '*': GetLocalTime( &curtime ); break;
       case '-':
       case '+': 
       {
         unsigned long number=0;
         unsigned __int64 changer;
         int count=0;
		 bool chang=TRUE;
         char numbuf[256];

         if (buf[i]=='-') chang=FALSE;
         i++;
         for (;;)
         {
           if ( IS_DIGIT(buf[i]) ) numbuf[count++]=buf[i++];
           else
           {
             numbuf[count]='\0';
             break;
           }
         }
         if (count==0) number=1;
         else number=atoi(numbuf);
         if (number > 32000 || number < 0)
         {
           _ftprintf(stdout,"\nSorry, maximum adjust - 32000 s/m/h or days\n");
           return 0;
         }
         switch (buf[i])
         {
           case 'd' : changer=number*86400; break;
           case 'h' : changer=number*3600;  break;
           case 'm' : changer=number*60;    break;
           case 's' : changer=number;       break;
           default  : changer=number*86400; break;
         }
         adjust_date(changer, chang, 0);
         continue;
       }
       default:
           _stprintf(temp,"$%c%c",buf[i],'\0'); break;
	       break;
     }
   }
   else { _stprintf(temp,"%c%c",buf[i],'\0'); }

   _tcscat(output,temp);
 }
 
 return 0;
}