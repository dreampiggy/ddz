#include <mysql/mysql.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

/*
    UsernameExits will return 3 kinds of values,
    -1 for error,
    1 for username has already exists
    0 for no such username
*/
int UsernameExist(char* un){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "2610207wu";
    char database[] = "DDZ";
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    sprintf(query,"SELECT COUNT(*) FROM users WHERE username='%s'", un);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    //row[0] is a char string
    if(atoi(row[0]) > 0){//this username has already exits
        return 1;
    }
    else   return 0;
    mysql_free_result(res);//!!!you must free the result after a query
    mysql_close(conn);
}

/*
    UserInsert will return two kinds of values
    -1 for error,
    1 for successful insert
*/
int UserInsert(char* un, char* pwd){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "root";
    char password[] = "2610207wu";
    char database[] = "DDZ";
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    sprintf(query,"INSERT INTO users(username, password)VALUES('%s','%s')", un, pwd);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if(mysql_affected_rows(conn)==1){//sucessfully insert an user
        return 1;
    }
    mysql_free_result(res); 
    mysql_close(conn);
}
