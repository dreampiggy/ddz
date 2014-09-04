//return 0 for continue the outer while
//return 2 for continue the inner while

//client main
while(1){
	BeginPrompt();//option list
	send(1);//choose option 1 for register
	if(recv==register){
		int flag=-1;
		while(1){
			flag = BeginWithRegis();
			if(flag==2)
				continue;
			else
				break;
		}
		if(flag==0)
			continue;
		else if(flag==-1)
			exit(1);//main break down
		else if(flag==1)
			break;//exit outer while
	}
}

//server main
while(1){
	if(recv==1){
		send(register);
		int flag=-1;
		while(1){
			flag = Register();
			if(flag==2)
				continue;
			else
				break;
		}
		if(flag==0)
			continue;
		else if(flag==-1)
			return -1;//main break down
		else if(flag==1)
			break;//exit outer while
	}
}

//client
BeginWithRegis(){
	send(username);
	if(recv==usernameexist)
		return 2;
	else if(recv==usernamenoexist){		
		while(1){
			gets(pwd);
			gets(repeat);
			if(pwd==repeat){
				send(pwd);
				if(recv==successregister)
					return 1;
				else	return -1;
			}
		}
	}
	else{
		return -1;
	}
}

//server
Register(){
	recv(username);
	if(usernameexist==1){
		send(usernameexist);
		return 2;
	}
	else if(usernameexist==0){
		send(usernamenoexist);
		recv(pwd);
		if(userinsert(username, pwd)==-1){//insert error
			send(failregister);
			return -1;
		}
		else{
			send(successregister)
			return 1//for success
		}
	}
	else{//-1 for query error
		return -1
	}
}