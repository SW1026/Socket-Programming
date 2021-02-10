#include <syslog.h>
#include <stdio.h>
#include <libpq-fe.h>

void connect_testdb();
void query_employee(PGconn * conn); 
