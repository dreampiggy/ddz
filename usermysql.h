#include <mysql/mysql.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

const char server[] = "localhost";
const char user[] = "root";
const char password[] = "2610207wu";
const char database[] = "DDZ";

/*
function list
1.UsernameExist
2.UserInsert
3.UserOnline
4.PasswordCorrect
5.StatusTurnOn
6.StatusTurnOff
*/


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
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;;
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

int UserOnline(char* un){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    sprintf(query,"SELECT status FROM users WHERE username='%s' LIMIT 1", un);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if( strcmp(row[0], "online")==0 )
        return 1;
    else if( strcmp(row[0], "offline")==0 )
        return 0;
    else{
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    mysql_free_result(res); 
    mysql_close(conn);
}

int PasswordCorrect(char* un, char* pwd){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    sprintf(query,"SELECT COUNT(*) FROM users WHERE username='%s' AND password='%s'", un, pwd);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    res = mysql_use_result(conn);
    row = mysql_fetch_row(res);
    if( atoi(row[0])>0 )
        return 1;
    else if( atoi(row[0])==0 )
        return 0;
    else{
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    mysql_free_result(res); 
    mysql_close(conn);
}

int StatusTurnOn(char* un){
    MYSQL *conn;
    MYSQL_RES *res;
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    sprintf(query,"UPDATE users SET status='online' WHERE username='%s' LIMIT 1", un);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if(mysql_affected_rows(conn)==1){//sucessfully insert an user
        return 1;
    }
    else{
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    mysql_free_result(res); 
    mysql_close(conn);
}

int StatusTurnOff(char* un){
    MYSQL *conn;
    MYSQL_RES *res;
    char query[100];   
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }
    sprintf(query,"UPDATE users SET status='offline' WHERE username='%s' LIMIT 1", un);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    if(mysql_affected_rows(conn)==1){//sucessfully insert an user
        return 1;
    }
    else{
        fprintf(stderr, "%s\n", mysql_error(conn));
        return -1;
    }

    mysql_free_result(res); 
    mysql_close(conn);
}
