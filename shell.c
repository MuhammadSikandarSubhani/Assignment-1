#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int stringlength(char* s){
	int count=0;
	while(s[count]!='\0'){
		count++;	
	}
	return count;
}
bool stringcompare(char* s, char* d){
	if(stringlength(s)==stringlength(d)){
		for(int i=0;d[i]!='\0';i++){		
			if(s[i]!=d[i]){
				return false;
			}
		}
		return true;
	}
	return false;
}

char* stringcopy(char* d,int check,char c){
	char* s=(char*)malloc(stringlength(d)+1);
	if(stringlength(d)!=0){
		int i=0; int k=0;
		if(check==1){
			s[0]=c;
			i++;
		}
		while(d[k]!='\0'){
			s[i]=d[k];
			i++;
			k++;
		}
		s[i]='\0';
	}
	return s;
}


void cddir(){
	char cwd[100];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		printf("%s",cwd);
	}
	else{
		perror("getcwd error");
	}
}

char* currentDir(){
	char cwd[150];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		int size=stringlength(cwd);
		char* raw=stringcopy(cwd,0,0);
		return raw;
	}
	else{
		perror("getcwd error");
		return 0;	
	}	
}

char* Input(){
	char c;
	printf("%c",'$');
	printf("%c",' ');
	c=getchar();
	if(c!='\n'){
		char buffer[150];
		scanf("%[^\n]%*c",buffer);
		int size=stringlength(buffer);
		if(size>0){
			char* s=stringcopy(buffer,1,c);
			return s;
		}
		else{
			printf("%s","size null");
			return 0;
		}
	}
	else{
		return NULL;
	}
}


void execute(char** cmd){
	int pid;
	pid=fork();
	if(pid<0)
	{
		printf("%s","exitcommand not exexuted\n");
	}
	else if(pid==0){
		if (execvp(cmd[0], cmd) < 0) { 
            		printf("%s","Command \'"); 
			printf("%s",cmd[0]);
			printf("%s","\' not found\n");
        	}
		exit(0);
	}
	else{
		wait(NULL);
	}
}


int numberofspace(char* cmd){
	int count=0;
	for(int j=0;cmd[j]!='\0'; j++){
		if(cmd[j]==' ')
			count++;
	}
	return count;
}


int checksym(char* s, int check){
	char d;
	if(check==0)
		d='|';
	else if(check==1)
		d='>';
	else if(check==2)
		d='<';
	int count=0;
	for(int i=0;s[i]!='\0';i++){
		if(s[i]==d){
			count++;
		}
	}
	return count;
}

char* getfile(char* s, int check, char d){
	
	if(s!=NULL){
		char* raw=(char*)malloc(stringlength(s)+1);
		if(check==0){
			int i=0;
			while(s[i]!=d && s[i]!='\0'){
				raw[i]=s[i];
				i++;
			}
			if(raw[i-1]==' ')
				raw[i-1]='\0';
			else
				raw[i]='\0';
		}
		else if(check==1){
			int i=0;
			while(s[i]!=d && s[i]!='\0')
				i++;
			i++;
			if(s[i]==' ' && s[i]!='\0')
				i++;
			int k=0;
			for(int j=i;s[j]!='\0';j++){
				raw[k]=s[j];
				k++;		
			}
			if(s[i]!='\0'){
				raw[k]='\0';
			}
			else{
				return 0;
			}
		}
		return raw;
	}
	else
		return 0;
}


void makepipe(char** cmd,char** cmd1){
	int pd[2];  
	pid_t pid; 
	pipe(pd);
  	 
    	pid = fork(); 	 
  
    	if (pid == 0) { 
        	close(pd[0]); 
        	dup2(pd[1], STDOUT_FILENO); 
        	close(pd[1]); 
		if(execvp(cmd[0],cmd)<0)
			perror("Command not executed");
		exit(1); 
	} 
	else { 

        	pid = fork();  
  
		if (pid == 0) { 
		    close(pd[1]); 
		    dup2(pd[0], STDIN_FILENO); 
		    close(pd[0]); 
		    if(execvp(cmd1[0],cmd1)<0)
			perror("Command not executed");
		    exit(1); 
		} 
		else { 
			close(pd[0]);
        		close(pd[1]);
			wait(NULL);
			wait(NULL); 
		} 
    	} 
}

void sortcommand(char** cmd,char* raw, int check){
	int out;

	pid_t pid;
	pid=fork();
	if(pid==0){
		if(check==1){
  			out = open(raw, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(out, 1);
		}
		else if(check==2){
			out= open(raw, O_RDONLY);
			dup2(out, 0); 
		}
		close(out);
  		execvp(cmd[0], cmd);
		exit(0);
	}
	else{
		wait(NULL);
	}
}

void sortpipe(char** cmd, char** cmd1, char* s){
	
	int pd[2];  
	pid_t pid; 
	close(1);
	int out= open(s, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
	pipe(pd);
  	 
    	pid = fork(); 	 
  
    	if (pid == 0) { 
        	close(pd[0]); 
        	dup2(pd[1], out); 
        	close(pd[1]); 
		if(execvp(cmd[0],cmd)<0)
			perror("Command not executed");
		close(pd[1]);
		exit(1); 
	} 
	else { 
  		wait(NULL);
		close(pd[1]); 
		dup2(pd[0], STDIN_FILENO); 
		close(pd[0]); 
		if(execvp(cmd1[0],cmd1)<0)
			perror("Command not executed");
		close(pd[0]);
		exit(1); 
    	} 
}


void command(char* cmd, bool mainflag){
	char* sym0; char* sym1; char* sym2; char* sym3; bool checkcmd=false; bool checkcmd1=false; bool checkcmd2=false; char ch;
	char gch;
	bool gcheck=false; 

	if(checksym(cmd,0)==1){
		checkcmd=true;
		ch='|';
		if(checksym(cmd,1)==1){
			gcheck=true;
			gch='>';
		}
	}
	else if(checksym(cmd,1)==1){
		checkcmd1=true;
		ch='>';	
	}
	else if(checksym(cmd,2)==1){
		checkcmd2=true;
		ch='<';
	}
	if(gcheck==true){
		char* ecmd[4];
		char* ecmd1[4];
		sym0=getfile(cmd,0,ch);
		sym1=getfile(cmd,1,ch);
		sym2=getfile(sym1,0,gch);
		sym3=getfile(sym1,1,gch);
		int esize=numberofspace(sym0);
		int esize1=numberofspace(sym2);
		
		char* sym4;char* sym5;
		int i;
		for(i=0;i<esize+1;i++){
			sym4=getfile(sym0,0,' ');
			ecmd[i]=sym4;
			sym0=getfile(sym0,1,' ');
		}
		ecmd[i]=NULL;
		for(i=0;i<esize1+1;i++){
			sym5=getfile(sym2,0,' ');
			ecmd1[i]=sym5;
			sym2=getfile(sym2,1,' ');
		}
		ecmd1[i]=NULL;
		pid_t pid;
		pid=fork();
		if(pid==0){
			sortpipe(ecmd,ecmd1,sym3);
		}
		else 
		{
			wait(NULL);
		}
		
	}
	else if(checkcmd==true || checkcmd1==true || checkcmd2==true){
		char* ecmd[4];
		char* ecmd1[4];
		sym0=getfile(cmd,0,ch);
		sym1=getfile(cmd,1,ch);
		int esize=numberofspace(sym0);
		int esize1=numberofspace(sym1);

		
		int i;
		for(i=0;i<esize+1;i++){
			sym2=getfile(sym0,0,' ');
			ecmd[i]=sym2;
			sym0=getfile(sym0,1,' ');
		}
		ecmd[i]=NULL;
		if(checkcmd==true){		
			
			for(i=0;i<esize1+1;i++){
				sym3=getfile(sym1,0,' ');
				ecmd1[i]=sym3;
				sym1=getfile(sym1,1,' ');
			}
			ecmd1[i]=NULL;
		}
		else{
			sym3=getfile(sym1,0,' ');
		}
		
		if(checkcmd==true){
			makepipe(ecmd,ecmd1);		
		}
		else{
			if(checkcmd1==true)
				sortcommand(ecmd,sym3,1);
			else if(checkcmd2==true)
				sortcommand(ecmd,sym3,2);
		}
		
	}
	else if(checksym(cmd,0)==0 && checksym(cmd,1)==0){
		
		char* s=getfile(cmd,0,' ');
		int sizeofcmd=2; bool flag=false;

		char* listcmd[sizeofcmd];
		listcmd[0]="exit";
		listcmd[1]="cd";
		
		int cmdnumber=-1; 

		for(int i=0;i<sizeofcmd && flag!=true;i++){
			flag=stringcompare(listcmd[i],s);
			if(flag==true)
				cmdnumber=i;
		}
		if(flag==true){
			if(cmdnumber==0){
				printf("terminated\n");
				exit(0);
				mainflag=true;
			}
			else if(cmdnumber == 1){
				char* raw=getfile(cmd,1,' ');
				char* fileDir=currentDir();
				chdir(raw);
				char* rawDir=currentDir();
				if(stringcompare(fileDir,rawDir)==true){
					printf("%s","bash: cd: \'");
					printf("%s",raw);
					printf("%s","\' No such File Directory exist\n");
				}		
			}
		}
		else{
			char* simplecmd[100];
			int i; 
			int size=numberofspace(cmd);
			sym0=NULL;
  
    			for (i = 0; i < size+1; i++) { 
        			sym0=getfile(cmd,0,' ');
				simplecmd[i]=sym0;
				cmd=getfile(cmd,1,' ');
    			} 
			simplecmd[i]=NULL;
			execute(simplecmd);
		}
	}
	else if(checksym(cmd, 0)>1 || checksym(cmd, 1)>1){
		
		char* cd=(char*)malloc(20);
		if(checksym(cmd, 0)>1)
			strcpy(cd, "has more than one pipe");
		if(checksym(cmd, 1)>1)
			strcpy(cd,"has more than one redirection"); 
		
		printf("%s", "That command contain ");
		printf("%s",cd);
		printf("%c",'\n');
	}
}
int main(){
	char* cmd; bool flag=false;
	while(flag!=true){
		cddir();
		cmd=Input();
		if(cmd!=NULL){
			command(cmd,flag);
		}
	}
	
}
