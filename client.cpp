#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  8887
#define BUFFER_SIZE 1024

void BeginPrompt(){
    puts("你想干嘛");
    puts("1.老子还特么没注册");
    puts("2.我牛逼我已经有帐号，我想登录");
    puts("3.我要匿名登录");
    puts("4.我媽叫我回家吃饭，我要退出");
}

/*
    when BWR:
    return 0, it means BWR will be restart 
    return -1 means BWR broke down with error
    return 1 indicates that everything is ok
*/
int BeginWithRegis(int sock_cli){
    char sendbuf[50], recvbuf[50], sendbuf2[50];
    puts("报上名来:");
    
    fscanf(stdin,"%s",sendbuf);
    while(strlen(sendbuf)==0){
        puts("什么都不说是how many意思？");
        fscanf(stdin,"%s",sendbuf);
    }

    send(sock_cli, sendbuf, sizeof(sendbuf), 0);
    recv(sock_cli, recvbuf, sizeof(recvbuf), 0);

    if(strcmp(recvbuf,"usernameexist")==0){
        puts("这帐号早就注册过了，傻逼");
        return 2;
    }
    else if(strcmp(recvbuf,"usernamenoexist")==0){
        puts("哎呀你这傻逼运气不错，这帐号没被注册");
        while(1){
            puts("说，你要啥口令:");
                    
            fscanf(stdin,"%s",sendbuf);
            while(strlen(sendbuf)==0){
                puts("什么都不说是how many意思？");
                fscanf(stdin,"%s",sendbuf);
            }

            puts("你刚说什么，再给我说一遍？");
            fscanf(stdin,"%s",sendbuf2);
            while(strlen(sendbuf2)==0){
                puts("什么都不说是how many意思？");
                fscanf(stdin,"%s",sendbuf2);
            }

            if(strcmp(sendbuf2, sendbuf)==0){
                send(sock_cli, sendbuf, sizeof(sendbuf), 0);
                recv(sock_cli, recvbuf, sizeof(recvbuf), 0);
                if(strcmp(recvbuf, "successregister")==0)
                    return 1;
                else
                    return -1;
            }
            else{
                puts("两次口令不一样，你特么在逗我？");
            }
        }
    }
    else{
        puts("Fuck! System Error!!!");
        return -1;
    }
}


int BeginWithLogin(int sock_cli){
    char sendbuf[50], recvbuf[50];
    puts("天王盖地虎，你的代号是什么？");
    
    fscanf(stdin,"%s",sendbuf);
    while(strlen(sendbuf)==0){
        puts("什么都不说是how many意思？");
        fscanf(stdin,"%s",sendbuf);
    }

    send(sock_cli, sendbuf, sizeof(sendbuf), 0);
    recv(sock_cli, recvbuf, sizeof(recvbuf), 0);
    if(strcmp(recvbuf,"inputpassword")==0){

		puts("宝塔镇河妖，报一下你的暗号：");
        fscanf(stdin,"%s",sendbuf);
        while(strlen(sendbuf)==0){
            puts("什么都不说是how many意思？");
            fscanf(stdin,"%s",sendbuf);
        }

        send(sock_cli, sendbuf, sizeof(sendbuf), 0);
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0);
        if(strcmp(recvbuf,"successlogin")==0){
			puts("可以进去了");
            puts("顺便说一下，我肥皂掉了。");
            return 1;
		}
        else if(strcmp(recvbuf, "passwordincorrect")==0){
			puts("暗号都说错！你难道是皇军派来的奸细？！");
            return 2;
		}
        else{
            return -1;
        }
    }
    else if(strcmp(recvbuf,"usernameonline")==0){
		puts("叫这个代号的人刚才已经进去了，你再等等把");
        return 2;
    }    
    else if(strcmp(recvbuf,"usernameexist")==0){
        puts("没有这个代号");
        return 2;
    }
    else{
        return -1;
    }
}

int main()
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip

    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];	
	//至此就成功连接服务器了
    int flag;
    while(1){
        BeginPrompt();
        fgets(sendbuf, sizeof(sendbuf), stdin);
        send(sock_cli, sendbuf, sizeof(sendbuf),0); ///发送
        recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收

        if(strcmp(recvbuf,"register")==0){
            int flag=-1;
            while(1){
                flag=BeginWithRegis(sock_cli);
                if(flag==2) continue;
                else    break;
            }
            if(flag==0)
                continue;
            else if(flag==-1)
                exit(1);
            else if(flag==1)
                break;
        }
        else if(strcmp(recvbuf,"login")==0){
            int flag=-1;
            while(1){
                flag=BeginWithLogin(sock_cli);
                if(flag==2)
                    continue;
                else
                    break;
            }
            if(flag==0)
                continue;
            else if(flag==1)
                break;
            else
                exit(1);
            }
    }
    printf("\n\n<============一段时间之后============>\n\n\n");
    puts("捡起了肥皂之后，你终于进来了。");
    close(sock_cli);
    return 0;
}
