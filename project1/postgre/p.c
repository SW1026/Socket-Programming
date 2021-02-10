#include "p.h"

void query_employee(PGconn * conn){
        const char * query = "SELECT 1 FROM test_table;";
        const PGresult * query_res = PQexec(conn, query);
        ExecStatusType query_status = PQresultStatus(query_res);

        char * query_error_code = PQresStatus(query_status);
        char * query_error_msg = PQresultErrorMessage(query_res);

        // 에러 코드로 검사하는 것으로 바꾸자.
        if ( (query_status == PGRES_BAD_RESPONSE) || (query_status == PGRES_NONFATAL_ERROR) || (query_status == PGRES_FATAL_ERROR)){
                printf("Query Failed.\n");
                printf("%s: %s",query_error_code,query_error_msg);
                syslog(LOG_ALERT,"QUERY ERRROR OCCURS.\n");
        }
        else{
                //현재 상태 : PGRES_TUPLES_OK (쿼리가 성공적으로 실행됨.)
                printf("Query Success.\n");
                printf("Success Code : %s\n",query_error_code);
        }
}


void connect_testdb(){

        PGconn * conn = PQconnectdb("user=postgres dbname=testdb");
        ConnStatusType conn_status = PQstatus(conn);
        if(conn_status == CONNECTION_BAD){
                printf("CONNECTION_BAD : Connection Failed.\n");
                char* conn_error_msg = PQerrorMessage(conn);
                printf("%s", conn_error_msg);
                syslog(LOG_ALERT,"CONNECTION ERROR OCCURS.\n");
                exit(1);
        }
        else{
                //CONNECTON_OK : 연결 성공됨.
                printf("CONNECTION_OK : Connection Success.\n");
                query_employee(conn);
                PQfinish(conn);
                exit(0);
        }

}


int main(int argc, char *argv[]){
        // syslog를 LOG_ERR 단계(0~3) 까지 로그 범위 설정.
	setlogmask(LOG_UPTO(LOG_ERR));
        openlog(argv[0], LOG_PERROR | LOG_CONS, LOG_USER);
	connect_testdb();
	closelog();

}

