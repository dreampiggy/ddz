//有个问题：我还不知道函数错误的时候会返回什么（应该是0 or NULL）
//但我现在依然会使用0表示函数正常运行之后的返回值
//-1表示函数运行错误
//client
while(1){
	BeginPrompt();
	send(2);
	if(recv==login){
		int flag=-1;
		while(1){
			flag=BeginwithLogin();
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

//server
while(1){
	if(recv==2){
		send(login);
		int flag=-1;
		while(1){
			flag=login();
			if(flag==2)
				continue;
			else
				break;
		}
		if(flag==0)
			continue;
		else if(flag==1)
			break;
		else{
			exit(1);
		}
	}
}

BeginwithLogin(){
	send(username);
	if(recv==inputpassword){
		send(password);
		if(recv == sucesslogin)
			return 1;
		else if(recv == passwordincorrect){
			return 2;
		}
		else{
			return -1;
		}
	}
	else if(recv==usernameonline){
		return 2;//continue to input username
	}
	else if(recv==usernameexist){
		return 2;
	}
	else{
		return -1;
	}
}

login(){
	recv(username);
	if(recv==--exit)//the command for exit during login is '--exit'
		return 0;
	if(usernameexist==1){
		if(usernameonline==0){
			send(inputpassword);
			recv(password);
			if(passwordcorrect==1){
				if(user_status_turn_on==1){
					send(sucesslogin);
					return 1;
				}
				else{
					send(faillogin);
					return -1;
				}
			}
			else if(passwordcorrect==0){//password incorrect
				send(passwordincorrect);
				return 2;
			}
			else{//error with password query
				send(fail)
				return -1;
			}
		}
		else if(usernameonline==1){
			send(usernameonline);
			return 2;
		}
		else{
			send(fail);
			return -1;
		}
	}
	else if(usernameexist==1){
		send(usernameexist);
	}
	else{
		send(fail);
		return -1;
	}
}
