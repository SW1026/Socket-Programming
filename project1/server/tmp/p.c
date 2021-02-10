#include "p.h"


//DB에 쿼리를 날리고 결과를 반환해주는 함수
char** query_employee(PGconn * conn, const char *query){
//      const char * query = "SELECT 1 FROM test_table;";

	//쿼리 결과들을 저장할 변수, 주소와 값 문제로 전역변수와  배열형태로 선언해주어야 함.
	static char res[100][128];
	//res가 전역변수라서 이 함수가 불릴때마다 초기화를 해주어야 함.
	for(int i=0; i<100;++i){
		memset(res[i],0,sizeof(char)*128);
	}
	//디비에 쿼리가 날라가는지의 결과를 반환하는 함수
        const PGresult * query_res = PQexec(conn, query);
        ExecStatusType query_status = PQresultStatus(query_res);

        char * query_error_code = PQresStatus(query_status);
        char * query_error_msg = PQresultErrorMessage(query_res);

	if((query_status == PGRES_BAD_RESPONSE) || (query_status == PGRES_NONFATAL_ERROR) || (query_status == PGRES_FATAL_ERROR)){
                printf("DB Server : Query Failed.\n");
                printf("%s: %s",query_error_code,query_error_msg);
                syslog(LOG_ALERT,"%s: %s",query_error_code,query_error_msg);
                syslog(LOG_ALERT,"QUERY ERROR OCCURS.\n");
        }
	else if(query_status == PGRES_EMPTY_QUERY){
		//아무것도 보내지 않았을 때.
		printf("DB Server : Query Failed.\n");
		//printf("%s\n",query_error_code);
		syslog(LOG_ALERT,query_error_code);
		syslog(LOG_ALERT,"QUERY ERROR OCCURS.");
	}
        else{//쿼리 성공
		char * dml = strtok(query," ");	
		if(strcmp(dml,"SELECT")==0){	//show 명령어 일 경우
			int rows = PQntuples(query_res);
			if(rows==0){	//빈 테이블 반환 시
				sprintf(res[0],"EMPTY RESULT");
				return res;
			}
			else{
				res[0][0] = rows;	//나중에 변환됨
				for(int row=0; row<rows; ++row){
					char* id = PQgetvalue(query_res, row,0); //row행의 id컬럼값
					char* name= PQgetvalue(query_res,row,1); //row행의 name 컬럼 반환
					char* age= PQgetvalue(query_res,row,2); //row행의 age컬럼 반환
					sprintf(res[row+1],"Query Result>> id : %s, name : %s, age : %s\n",id, name, age);
				}
                		printf("DB Server : Query Success.\n");
				return res;
			}
		}
                //현재 상태 : PGRES_TUPLES_OK (쿼리가 성공적으로 실행됨.)
                printf("DB Server : Query Success.\n");
		sprintf(res[0],"NOT SELECT");
		return res;
        }
	//쿼리실패
	sprintf(res[0], "NOT QUERY");
	return res;
}


//DB와 연결해주는 함수
PGconn* connect_testdb(){

        PGconn * conn = PQconnectdb("user=postgres dbname=testdb");
        ConnStatusType conn_status = PQstatus(conn);
        if(conn_status == CONNECTION_BAD){
		//연결 실패
                printf("DB connection Failed.\n");
                char* conn_error_msg = PQerrorMessage(conn);
                printf("%s", conn_error_msg);
		syslog(LOG_ALERT,conn_error_msg);
                syslog(LOG_ALERT,"DB CONNECTION ERROR OCCURS.\n");
                exit(1);
        }
        else{
                //CONNECTON_OK : 연결 성공됨.
                printf("DB connection Success.\n");
               	return(conn);
		// exit(0);
        }

}

