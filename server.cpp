#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "login.cpp"
using namespace std;

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024

/*
    RegisterUser return 3 kinds of values
    1: success
    2: username duplicated
    -1: error
*/
int RegisterUser(int conn){
    char username[50], pwd[20];
    recv(conn, username, sizeof(username), 0);
    int sta = UsernameExist(username);
    if(sta==1){
        send(conn, "usernameexist", sizeof("usernameexist"), 0);
        return 2;
    }          
    else if(sta==0){
        send(conn, "usernamenoexist", sizeof("usernamenoexist"), 0);
        recv(conn, pwd, sizeof(pwd), 0);
        if(UserInsert(username, pwd)==1){
            send(conn, "successregister", sizeof("successregister"), 0);
            return 1;
        }
        else{
            send(conn, "failregister", sizeof("failregister"), 0);
            return -1;
        }
    }  
    else{
        return -1;
    }
}


/*
有几个安全性的问题需要后期的时候注意:
1.因为密码存放在char数组，密码的长度过长会超出数组大小限制
所以要限制密码长度
2.
*/

int main(){
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }

	//客户端发回去的信息不带回车
    //一开始询问用户要注册登录，已有账户登录，匿名登录还是退出
    int flag=-1;
    while(1){
        memset(buffer, 0, sizeof(buffer));
        recv(conn, buffer, sizeof(buffer), 0);
        if(strcmp(buffer,"1\n")==0){//
            flag=-1;
            send(conn, "register", sizeof("register"), 0);
            while(1){
                flag = RegisterUser(conn);
                if(flag==2)
                    continue;
                else
                    break;
            }
            if(flag==0)
                continue;
            else if(flag==-1)
                exit(1);
            else
                break;
    	}
        else
            exit(1);
    }

    puts("fuck!! you're in !!");

    close(conn);
    close(server_sockfd);
    return 0;
}




