//http://src.gnu-darwin.org/src/bin/pwd/pwd.c.html 출처
//과제의 pwd는 사용 후 쉘의 폴더가 전환되어버리는 문제로 사용하지 않음.
#if 0
#ifndef lint
static char const copyright[] =
"@(#) Copyright (c) 1991, 1993, 1994\n\
	The Regents of the University of California.  All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)pwd.c	8.3 (Berkeley) 4/1/94";
#endif /* not lint */
#endif
#include <sys/cdefs.h>

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *getcwd_logical(void);
void usage(void);
void printPWD();
char *p;
