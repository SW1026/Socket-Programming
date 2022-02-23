#include "postgres.h"

//DB에 쿼리를 날리고 결과를 반환해주는 함수
struct Result* 
query_employee(PGconn * conn, const char *query)
{
	const PGresult* query_res = NULL; 
	ExecStatusType query_status; 
	char* query_error_code = NULL;
	char* query_error_msg = NULL; 

	//쿼리 결과들을 저장할 변수, 주소와 값 문제로 전역변수와  배열형태로 선언해주어야 함.
	//res가 전역변수라서 이 함수가 불릴때마다 초기화를 해주어야 함.
	int i;
	for(i = 0 ; i < 128 ; ++i)
	{
		memset(result.res[i], 0, sizeof(char)*128);
	}

	//디비에 쿼리가 날라가는지의 결과를 반환하는 함수
	query_res = PQexec(conn, query);
	query_status = PQresultStatus(query_res);
	query_error_code = PQresStatus(query_status);
	query_error_msg = PQresultErrorMessage(query_res);

	if((query_status == PGRES_BAD_RESPONSE) || (query_status == PGRES_NONFATAL_ERROR) || (query_status == PGRES_FATAL_ERROR))
	{
        syslog(LOG_ERR,"----- DB Server : QUERY ERROR OCCURS -----\n");
		syslog(LOG_ERR, "%s\n", query_error_code);
		syslog(LOG_ERR, "%s", query_error_msg);
    }
	else if(query_status == PGRES_EMPTY_QUERY)
	{ 
		//클라이언트에서 강제종료(Ctrl+c)했을 시
		//syslog(LOG_ERR,"----- DB Server : QUERY ERROR OCCURS -----\n");
		//syslog(LOG_ERR,"%s\n",query_error_code);
		//syslog(LOG_ERR,"%s",query_error_msg);
		//좀비서버가 되서 문제임..
		close_socket(conn);//자식서버 통신 마무리
		printf("--------------------------------------------------\n");
		
		exit(1); //클라이언트에서 강제종료하면, 이 시점에서 서버도 자동으로 종료된다. 명시적으로 써준 것.
	}
    else
	{
		// 쿼리 설공
		char* dml = NULL;

		dml = strtok(query," ");	
		if(strncmp(dml, "SELECT", strlen("SELECT")) == 0)
		{
			//show 명령어 일 경우
			result.rows = PQntuples(query_res);
			if(result.rows==0)
			{	//빈 테이블 반환 시
				strcpy(result.type, "EMPTY RESULT");
				return &result;
			}
			else if(result.rows > 0)
			{
				int row;
				char* id = NULL;
				char* name = NULL;
				char* age = NULL;
				strcpy(result.type, "SELECT RESULT");
				for(row = 0; row < result.rows ; ++row)
				{
					id = PQgetvalue(query_res, row,0);
					name= PQgetvalue(query_res,row,1);
					age= PQgetvalue(query_res,row,2);

					sprintf(result.res[row], "Query Result>> id : %s, name : %s, age : %s\n", id, name, age);
				}
                printf("DB Server : Query Success.\n");

				return &result;
			}
		}

        //현재 상태 : PGRES_TUPLES_OK (쿼리가 성공적으로 실행됨.)
        printf("DB Server : Query Success.\n");
		strcpy(result.type,"NOT SELECT");

		return &result;
	}
	//쿼리실패(사실상 실패해도)
	strcpy(result.type, "FAIL QUERY");

	return &result;
}


//DB와 연결해주는 함수
PGconn* 
connect_testdb()
{
	char* conn_error_msg = NULL;
	PGconn * conn = NULL;
	ConnStatusType conn_status;

	conn = PQconnectdb("user=postgres dbname=testdb");
	conn_status = PQstatus(conn);
	if(conn_status == CONNECTION_BAD)
	{
		printf("DB Connection Failed.\n\n");
		conn_error_msg = PQerrorMessage(conn);
		syslog(LOG_ERR,"---- DB Server : CONNECT ERROR OCCURS ----\n");
		syslog(LOG_ERR, conn_error_msg);
		printf("--------------------------------------------------\n");

		//클라이언트에게 메세지를 보내주자
		return(conn);
	}
	else
	{
		// CONNCTON_OK : 연결 성공됨.
		printf("DB Connection Success.\n");
		printf("--------------------------------------------------\n");
		return(conn);
	}
}

