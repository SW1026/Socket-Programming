#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <errno.h>

struct Result{
	char res[128][128];
	int rows;
	char type[10];
} result;
