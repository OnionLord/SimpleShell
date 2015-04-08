#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<stdlib.h>
#include<stdio.h>
#include "vfolder.h"


//한글 명령어 지원
//child process관리 -> signal handler이용(SIGCHLD)
//ctrl+c 입력시 셀 종료 방시 -> signal handler이용(SIGINT)
void viewHelp(char *);

int main()
{
    char *s_root;
    char *arglist[256];
    int arg_cnt = 1;
    char a[256];
    char *line_p;
    char *arg;
    char cd[3];
    int size, size1;
    pid_t pid;
    void sig_handle(int);
    void child_waiter(int);
    signal(SIGCHLD, child_waiter); //child생성시 wait
    signal(SIGINT, sig_handle); //ctrl+c 처리. 눌러도 쉘이 종료되지는 않는다.
    printPWD();//현재 폴더 표시. vfolder.h
    s_root = p;//쉘이 존재하는 폴더 보존.    

    printf("A Shell As Project [Version 1.0.0]\n");
    printf("20142 System Programming [ELEC462004]\n\n");
    
    while(1)
    {
        arg_cnt = 0;
        printPWD(); //vfolder.h, 현재 폴더 출력.
        printf("%s>",p);
		
	fgets(a, 256, stdin); 
      	if ((line_p=strchr(a,'\n'))!=NULL)*line_p='\0'; //fgets시 마지막에 개행문자 남는거 제거.
	
	/*else if(strcmp(a,"qc")==0)
        {
            execlp(strncat(s_root,"/qc",3),"qc", NULL);
        }*/

        if(strcmp(a,"exit") == 0||strcmp(a,"종료") == 0)//이 shell종료.
        {
            write(1, "Session Out\n", 13);
            wait(NULL);
            kill(pid, SIGKILL);
            exit(0);
            return 1;
        }
        
        arg = strtok(a," ");
        arglist[0] = a;
        arglist[1] = 0;
        while(1)
        {
            if(arg == NULL)
            {
                break;
            }
            else
            {
                arglist[arg_cnt] = arg;
                arg_cnt++;
                arglist[arg_cnt] = 0;
            }
            arg = strtok(NULL, " ");
        }
	if(strcmp(a,"help") == 0||strcmp(a,"도움말") == 0)
        {
	     viewHelp(arglist[1]);
	     continue;
        }
        if(strcmp(a,"cd") == 0||strcmp(a,"이동") == 0) //fork후 디렉토리 변경시 exit를 여러번 해야하므로fork전에 cd를 처리.
        {
            if(arglist[1] == NULL)
            {
                
            }
            else
            {
                if(chdir(arglist[1])==-1)
                {
                    write(1, "No directory\n",14);
                }
            }
            continue;//fork를 하지 않도록 한다.
        }
        pid = fork();
        if(pid < 0)
        {
            write(1, "fork Fail\n", 11);
        }
        else if(pid == 0)//실행할 명령어들
        {
            
            if(strcmp(a,"ls")==0||strcmp(a,"목록")==0)
            {
                execvp(strncat(s_root,"/ls",3), arglist);
            }
            else if(strcmp(a,"copy")==0||strcmp(a,"복사")==0)
            {
                execvp(strncat(s_root,"/cp",3), arglist);
            }
	    else if(strcmp(a,"calc")==0||strcmp(a,"계산기")==0)
            {
                execvp(strncat(s_root,"/calc",5), arglist);
            }
            else if(strcmp(a,"lotto")==0||strcmp(a,"로또")==0)
            {
                execvp(strncat(s_root,"/lotto",6), arglist);
            }
            else if(strcmp(a,"chatserver")==0||strcmp(a,"채팅서버")==0)
            {
                execvp(strncat(s_root,"/chatserver",11), arglist);
            }
		else if(strcmp(a,"wc")==0||strcmp(a,"단어세기")==0)
            {
                execvp(strncat(s_root,"/wc",3), arglist);
            }
            else if(strcmp(a,"chatclient")==0||strcmp(a,"채팅클라")==0)
            {
                execvp(strncat(s_root,"/chatclient",11), arglist);
            }
            else if(strcmp(a,"ps")==0||strcmp(a,"프로세스")==0)
            {
                execlp("/bin/ps", "ps", NULL);
            }
            else if(strcmp(a,"clear")==0||strcmp(a,"정리")==0)
            {
                execlp("/usr/bin/clear", "clear", NULL);
            }
	else if(strcmp(a,"cat")==0||strcmp(a,"읽기")==0)
            {
		execvp("/bin/cat", arglist);
            }
            else if(strcmp(a,"top")==0)
            {
                execlp("/usr/bin/top","top", NULL);
            }
            /*else if(strcmp(a, "cd") == 0)
            {
                //arg = strtok(NULL, " ");
                
                if(arglist[1] == NULL)
                {
                    exit(1);
                }
                else
                {
                    if(chdir(arglist[1])==-1)
                    {
                        write(1, "No directory\n",14);
                    }
                }
            }*/
            else if(strcmp(a, "")==0)
            {
            }
            else
            {
                write(1,"Wrong command\n",14);
                exit(1);
            }
            
        }
        else
        {
            wait(NULL);
            kill(pid, SIGKILL);
        }
        
        
    }
    
    
	return 0;
}

void sig_handle(int signum)
{
	printf("\n");
	printPWD();
}
void child_waiter(int signum)
{
    while(waitpid(-1,NULL,WNOHANG) >0);
}
void viewHelp(char *a)
{
    if(a == NULL)
    {
	printf("=======================도움말=======================\n");
        printf("ls/목록             : 디렉터리 및 파일의 리스트 확인\n");
        printf("copy/복사          : 해당 파일을 복사\n");
        printf("ps/프로세스         : 현재 실행중인 프로세스 확인\n");
        printf("clear/정리          : 현재 화면 정리\n");
        printf("lotto/로또          : 가상 로또 추첨.\n");
        printf("chatserver/채팅서버 : 채팅 서버를 개방\n");
        printf("chatclient/채팅클라 : 채팅 서버로 접속\n");
	printf("calc/계산기         : 괄호 포함 사칙연산 지원하는 정수 계산\n");
	printf("wc/단어세기         : 파일에 있는 단어, 문자에 대한 통계 지\n");
        printf("exit/종료           : ASAP Shell 종료\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"ls")==0||strcmp(a,"목록")==0)
    {
        printf("=======================도움말=======================\n");
        printf("ls/목록 + 인자1 + 인자2 + 인자3 + ... + 인자n\n");        
	printf("-a : 숨김 파일 표시\n");
        printf("-l : 파일 상세내역 표시\n");
        printf("-R : 하위 폴더의 내용도 조회\n");
	printf("-f : 정렬 하지 않고 출력\n");
	printf("-i : iNode 출력\n");
        printf("폴더 경로\n");
	printf("인자는 없어도 되며 인자 수의 제한이 없음.\n");
        printf("====================================================\n");
    }
	else if(strcmp(a,"wc")==0||strcmp(a,"단어세기")==0)
    {
        printf("=======================도움말=======================\n");
        printf("ls/목록 + 인자1 + 파일명 + 파일명 + ... + 파일명n\n");        
	printf("-w : 파일에 있는 단어를 셈(기본)\n");
        printf("-l : 파일에 있는 줄 수를 셈.\n");
        printf("-c : 파일에 있는 문자 수를 셈\n");
	printf("-m : 파일에 있는 캐릭터 수를 셈\n");
	printf("-L : 각 파일에 있는 가장 긴 라인 번호를 출력\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"copy")==0||strcmp(a,"복사")==0)
    {
        printf("=======================도움말=======================\n");
        printf("copy/복사+ 인자1 + 인자2\n");        
	printf("인자1 : 원본 파일\n");
        printf("인자2 : 대상 파일\n");
        printf("인자가 입력이 되지 않았을 경우 실행후 표준 입력으로 받음\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"lotto")==0||strcmp(a,"로또")==0)
    {
        printf("=======================도움말=======================\n");
        printf("lotto/로또\n");    
	printf("모의 로또 추첨 프로그램.\n");    
	printf("6 숫자가 보여지고 나서 엔터를 누르면 종료.\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"chatserver")==0||strcmp(a,"채팅서버")==0)
    {
        printf("=======================도움말=======================\n");
        printf("chatserver/채팅서버 + 인자1\n");        
	printf("인자1 : 포트번호\n");
        printf("채팅 서버를 개방\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"chatclient")==0||strcmp(a,"채팅클라")==0)
    {
        printf("=======================도움말=======================\n");
        printf("chatclient/채팅클라 + 인자1 + 인자2 + 인자3\n");        
	printf("인자1 : 서버 이름/IP\n");
        printf("인자2 : 서버 포트\n");
        printf("인자3 : 대화명\n");
        printf("채팅 서버로 접속\n");
        printf("====================================================\n");
    }
    else if(strcmp(a,"ps")==0||strcmp(a,"프로세스")==0)
    {
        printf("=======================도움말=======================\n");
        printf("ps/프로세스 + 인자1 + 인자2 + 인자3\n");   
        printf("현재 실행중인 프로세스를 보여 줍니다.\n");     
        printf("====================================================\n");
    }
    else if(strcmp(a,"clear")==0||strcmp(a,"정리")==0)
    {
        printf("=======================도움말=======================\n");
        printf("clear/정리\n");   
        printf("현재 화면을 비워줍니다.\n");     
        printf("====================================================\n");
    }
    else if(strcmp(a,"calc")==0||strcmp(a,"계산기")==0)
    {
        printf("=======================도움말=======================\n");
        printf("calc/계산기\n");   
        printf("프로그램 실행 후 괄호를 포함한 식을 넣습니다.\n");
        printf("모두 입력한 후 Ctrl+D+D를 입력하면 식의 결과가 출력이 됩니다.\n");  
        printf("현재는 정수만 입력이 가능합니다.\n");   
        printf("====================================================\n");
    }
    else if(strcmp(a,"exit")==0||strcmp(a,"종료")==0)
    {
        printf("=======================도움말=======================\n");
        printf("exit/종료\n");   
        printf("ASAP Shell을 종료합니다.\n");     
        printf("====================================================\n");
    }
    else if(strcmp(a,"help")==0||strcmp(a,"도움말")==0)
    {
        printf("=======================도움말=======================\n");
        printf("help/도움말 + 인자1\n");   
        printf("인자1 : 대상 명령어\n");     
        printf("====================================================\n");
    }
    else
    {
        printf("=======================도움말=======================\n");
        printf("[help/도움말 + 올바른 명령어]로 입력해 주세요!\n");   
        printf("====================================================\n");
    }
}
