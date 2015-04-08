//http://src.gnu-darwin.org/src/bin/pwd/pwd.c.html 출처
//과제의 pwd는 사용 후 쉘의 폴더가 전환되어버리는 문제로 사용하지 않음.

#include "vfolder.h"

void printPWD()
{
	int physical;
	int ch;
	

	// 4207130
	physical = 0;
	
	/*
	 * If we're trying to find the logical current directory and that
	 * fails, behave as if -P was specified.
	 *
	 * 4701537: Only fall back if $PWD was actually wrong. We want to
	 * fail if the path is too long (errno == ENAMETOOLONG).
	 */
	if ((!physical && (p = getcwd_logical()) != NULL) ||
	    ((physical || errno == ENOENT) && (p = getcwd(NULL, 0)) != NULL))
	{
	}
	else
	{
		err(1, ".");
	}

}

void
usage(void)
{

	(void)fprintf(stderr, "usage: pwd [-L | -P]\n");
  	exit(1);
}

static char *
getcwd_logical(void)
{
	struct stat lg, phy;
	char *pwd;

	/*
	 * Check that $PWD is an absolute logical pathname referring to
	 * the current working directory.
	 */
	if ((pwd = getenv("PWD")) != NULL && *pwd == '/') {
		if (stat(pwd, &lg) == -1 || stat(".", &phy) == -1)
			return (NULL);
		if (lg.st_dev == phy.st_dev && lg.st_ino == phy.st_ino)
			return (pwd);
	}

	errno = ENOENT;
	return (NULL);
}
