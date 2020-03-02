#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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

char* stringcopy(char* d){
	char* s=(char*)malloc(stringlength(d)+1);
	if(stringlength(d)!=0){
		int i;
		for(i=0;d[i]!='\0';i++)
			s[i]=d[i];
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
		char* raw=stringcopy(cwd);
		return raw;
	}
	else{
		perror("getcwd error");
		return 0;	
	}	
}

char* Input(){
	char buffer[150];
	printf("%c",'$');
	printf("%c",' ');
	scanf("%[^\n]%*c",buffer);
	int size=stringlength(buffer);
	if(size>0){
		char* s=stringcopy(buffer);
		return s;
	}
	else{
		printf("%s","size null");
		return 0;
	}
}

char* rawData(char* cmd){
	char* raw=(char*)malloc(stringlength(cmd)+1);
	int i=0;
	while(cmd[i]!=' ')
		i++;
	i++;
	int k=0;
	for(int j=i;cmd[j]!='\0'; j++){
		raw[k]=cmd[j];
		k++;
	}
	raw[k]='\0';
	return raw;
}

void execute(char* cmd, int cmdnumber){
	int pid;
	pid=fork();
	if(pid<0)
	{
		printf("%s","exitcommand not exexuted\n");
	}
	else if(pid==0){
		if(cmdnumber==1){
			int ret = execlp("/bin/ls","ls","-1",(char *)0);
		}
		else if(cmdnumber==5){
			int ret = execlp("/bin/touch","touch",cmd,(char *)0);		
		}
		else{
			char* s=rawData(cmd);
			if(cmdnumber==2){
				int ret = execlp("/bin/mkdir","mkdir",s,(char *)0);
			}
			else if(cmdnumber==3){
				int ret = execlp("/bin/rmdir","rmdir",s,(char *)0);		
			}
		}
	}
	else{
		wait(NULL);
	}
}


char* touchseries(char* cmd,int n){
	char* raw=(char*)malloc(stringlength(cmd)+1);
	int i=0;
	while(n>=0){
		if(cmd[i]==' ')
			n--;
		i++;
	}
	int k=0;
	for(int j=i;cmd[j]!=' ' && cmd[j]!='\0'; j++){
		raw[k]=cmd[j];
		k++;
	}
	raw[k]='\0';
	return raw;
}

int numberofspace(char* cmd){
	int count=0;
	for(int j=0;cmd[j]!='\0'; j++){
		if(cmd[j]==' ')
			count++;
	}
	return count;
}


char* getcommand(char* cmd){
	char* s=(char*)malloc(stringlength(cmd)+1);
	int i; bool flag=false;
	for(i=0;cmd[i]!='\0'; i++){
		if(cmd[i]==' '){
			break;
		}
		s[i]=cmd[i];
	}
	s[i]='\0';
	return s;
}

bool checksym(char* s, int check){
	char d;
	if(check==0)
		d='|';
	else if(check==1)
		d='>';
	for(int i=0;s[i]!='\0';i++){
		if(s[i]==d){
			return true;
		}
	}
	return false;
}

char* getfile(char* s, int check, char d){
	
	char* raw=(char*)malloc(stringlength(s)+1);
	if(check==0){
		int i=0;
		while(s[i]!=d){
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
		while(s[i]!=d)
			i++;
		i++;
		if(s[i]==' ')
			i++;
		int k=0;
		for(int j=i;s[j]!='\0';j++){
			raw[k]=s[j];
			k++;		
		}
		raw[k]='\0';
	}
	return raw;
}

void makepipe(char* cmd,char* s,char* d){
	int pd[2];  
	pid_t fd1, fd2; 
	pipe(pd);
  	 
    	p1 = fork(); 	 
  
    	if (fd1 == 0) { 
        	close(pd[0]); 
        	dup2(pd[1], STDOUT_FILENO); 
        	close(pd[1]); 
  
        	if (execvp(cmd,&s) < 0) { 
            		printf("\nCould not execute command 1"); 
            		exit(0); 
        	} 
	} 
	else { 

        	fd2 = fork();  
  
		if (fd2 == 0) { 
		    close(pd[1]); 
		    dup2(pd[0], STDIN_FILENO); 
		    close(pd[0]); 
		    if (execvp(cmd,&d) < 0) { 
		        printf("\nCould not execute command 2"); 
		        exit(0); 
		    } 
		} 
		else { 
		    wait(NULL); 
		    wait(NULL); 
		} 
    	} 
}

void sortcommand(char* sym0,char* sym1,char* sym2){
	FILE* file; FILE* file1;
	file=fopen(sym1,"r");
	char buffer[100];
	if(file!=NULL){
		while(fgets(buffer,100, file) != NULL ) 
		  	continue;
		fclose(file);				
		if(stringcompare(sym0,"sort")){
			int size=stringlength(buffer);
			for(int i=0;i<size-1;i++){
				for(int j=0;j<size-i-1;j++){
					if(buffer[j]>buffer[j+1]){
						if(buffer[j+1]!='\n'){
							char temp=buffer[j];
							buffer[j]=buffer[j+1];
							buffer[j+1]=temp;	
						}	
					}				
				}				
			}
		}
		file1=fopen(sym2,"w");
		if(stringlength(buffer)>0){
			fputs(buffer,file1);			
		}
		fclose(file1);
	}
	else{
		printf("%s","sort: cannot read: ");
		printf("%s",sym1);
		printf("%s",": Not such file directory exist\n");
	}
}

void command(char* cmd, bool mainflag){
	
	if(checksym(cmd,0)){
		char* sym0=getcommand(cmd);
		char* sym=rawData(cmd);
		char* sym1=getfile(sym,0,'|');
		char* sym2=getfile(sym,1,'|');
		makepipe(sym0,sym1,sym2);
	}
	else if(checksym(cmd,1)){
		char* sym0=getcommand(cmd);// get command such as sort
		if(stringcompare(sym0,"sort") || stringcompare(sym0,"cat")){
			char* sym=rawData(cmd);// get all data except command
			char* sym1=getfile(sym,0,'>');// get first file before symbol >
			char* sym2=getfile(sym,1,'>');// get second file which is written after symbol >
			sortcommand(sym0,sym1,sym2);
		}
		else{
			printf("%s","Command: \'"); 
			printf("%s",sym0);
			printf("%s","\' not found\n");
		}
	}
	else{
		char* s=getcommand(cmd);
		int sizeofcmd=9; bool flag=false;

		char* listcmd[sizeofcmd];
		listcmd[0]="exit";
		listcmd[1]="ls";
		listcmd[2]="mkdir";
		listcmd[3]="rmdir";
		listcmd[4]="cd";
		listcmd[5]="touch";
		listcmd[6]="clear";
		listcmd[7]="pwd";
		listcmd[8]="rm";
		
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
			else if(cmdnumber == 6){
				system("clear");	
			}
			else if(cmdnumber == 7){
				cddir();
				printf("%c",'\n');		
			}
			else if(cmdnumber == 8){
				for(int i=0;i<numberofspace(cmd);i++){
					char* raw=touchseries(cmd,i);
					if(remove(raw)!=0){
						printf("%s","rm: cannot remove: \'");
						printf("%s",raw);
						printf("%s","\' No such File Directory exist\n");				
					}	
				}		
			}
			else if(cmdnumber == 1){
				execute(cmd,cmdnumber);
			}
			else if(cmdnumber == 2 || cmdnumber == 3){
				execute(cmd,cmdnumber);
			}
			else if(cmdnumber == 4){
				char* raw=rawData(cmd);
				char* fileDir=currentDir();
				chdir(raw);
				char* rawDir=currentDir();
				if(stringcompare(fileDir,rawDir)==true){
					printf("%s","bash: cd: \'");
					printf("%s",raw);
					printf("%s","\' No such File Directory exist\n");
				}		
			}
			else if(cmdnumber == 5){
				for(int i=0;i<numberofspace(cmd);i++){
					char* raw=touchseries(cmd,i);
					execute(raw,cmdnumber);	
				}	
			}
		}
		else{
			printf("%s","Command: \'"); 
			printf("%s",s);
			printf("%s","\' not found, but can be install by adding in command List \n");
		}
	}
}
int main(){
	char* cmd; bool flag=false; 
	while(flag!=true){
		cddir();
		cmd=Input();
		command(cmd,flag);
	}
	
}
