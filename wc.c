#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<pthread.h>

int total_all=0;
int total_word=0;
int total_char=0;
int total_line=0;
int find_longline=0;
int check=0;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int ac, char *av[])
{
	pthread_t t;
	int che;
	int i;

	void *count_all(void *);
	void *count_char(void *);
	void *count_word(void *);
	void *count_line(void *);
	void *count_find(void *);

	check = ac-3;
	
	if(ac < 2)
	{
		printf("입력 예시 : wc -(매개변수) 파일명1 파일명2 ...\n혹은 wc 파일명1 파일명2 ...");
	}

	if(!strcmp(av[1],"-c"))
	{
		che =1;
		for(i=2;i<ac;i++){
		pthread_create(&t,NULL,count_all,(void*) av[i]);
		pthread_join(t,NULL);
		}
	}
	else if(!strcmp(av[1],"-m"))
	{
		che =2;
		for(i=2;i<ac;i++){
		pthread_create(&t,NULL,count_char,(void *)av[i]);
		pthread_join(t,NULL);
		}
	}
	else if(!strcmp(av[1],"-l"))
	{
		che =3;
		for(i=2;i<ac;i++){
		pthread_create(&t,NULL,count_line,(void *)av[i]);
		pthread_join(t,NULL);
		}
	}
	else if(!strcmp(av[1],"-w"))
	{
		che=4;
		for(i=2;i<ac;i++){
		pthread_create(&t,NULL,count_word,(void*) av[i]);
		pthread_join(t,NULL);
		}
	}
	else if(!strcmp(av[1],"-L"))
	{
		che =5;
		for(i=2;i<ac;i++){
		pthread_create(&t,NULL,count_find,(void *)av[i]);
		pthread_join(t,NULL);
		}
	}
	else 
	{
		che=4;
		for(i=1;i<ac;i++){
		pthread_create(&t,NULL,count_word,(void*) av[i]);
		pthread_join(t,NULL);
		}
	}

	switch(che)
	{
		case 1:
			printf("모든 문자 수 : %d ",total_all);
			break;
		case 2:
			printf("모든 캐릭터 수 : %d",total_char);
			break;
		case 3:
			printf("모든 라인 수 : %d",total_line);
			break;
		case 4:
			printf("모든 단어 수 : %d",total_word);
			break;
		case 5:
			printf("가장 긴 라인 : %d",find_longline);
			break;
	}

	puts("");
	return 0;
}	

void *count_all(void *f)
{
	//total_all
	char *filename = (char *)f;
	FILE *fp;
	int c;
	int count=0;
	if((fp=fopen(filename,"r"))!=NULL)
	{
		while((c=getc(fp))!=EOF)
		{
			if(c=='\n' || c==' ')
			{

			}
			else
			   	count++;
		}
		fclose(fp);
		total_all+=count;
		if(check!=0){
			printf("%s : %d\n",filename,count);
		}
		check++;
	}
	else
		perror(filename);

	return NULL;
}

void *count_char(void *f)
{
	//total_char
	char *filename = (char*)f;
	FILE *fp;
	int c;
	int count=0;
	if((fp=fopen(filename,"r"))!=NULL)
	{
		while((c=getc(fp))!=EOF)
		{
			count++;
		}
		fclose(fp);
		total_char +=count;
		if(check!=0)
		{
			printf("%s : %d\n",filename,count);
		}
	}
	else
		perror(filename);

	return NULL;
}

void *count_line(void *f)
{
	//total_line
	char *filename = (char *)f;
	FILE *fp;
	int c;
	int count=0;
	int line=0;
	if((fp=fopen(filename,"r"))!=NULL)
	{
		while((c=getc(fp))!=EOF)
		{
			if(c!='\n')
			{

			}
			else
				count++;
		}
		if(check!=0){
		printf("%s : %d\n",filename,count);
		}
		total_line+=count;
	}
}

void *count_find(void *f)
{
	//	find_longline
	char *filename = (char *)f;
	FILE *fp;
	int c;
	int count=0;
	int line=0;
	int maxcount=-1;
	if((fp=fopen(filename,"r"))!=NULL)
	{
		while((c=getc(fp))!=EOF)
		{
			if(c!='\n')
			{
				count ++;
			}
			else if(c=='\n'&& (maxcount<=count))
			{
				line++;
				maxcount=count;
				count=0;
				find_longline=line;
			}
			else if(c=='\n' && (maxcount>=count))
			{
				line++;
				count=0;
			}
		}
		if(check!=0){
		printf("%s : %d\n",filename,find_longline);
		}
		fclose(fp);
	}
	else
		perror(filename);
	
	return NULL;
}
void *count_word(void *f)
{
	char *filename = (char*)f;
	FILE *fp;
	int c, prevc = '\0';
	int count=0;
	if((fp = fopen(filename,"r"))!=NULL)
	{
		while((c=getc(fp))!=EOF)
		{
			if(!isalnum(c) && isalnum(prevc)){
				pthread_mutex_lock(&counter_lock);
				count++;
				pthread_mutex_unlock(&counter_lock);
			}
			prevc = c;
		}
		total_word +=count;
		if(check!=0)
		{
			printf("%s : %d\n",filename,count);
		}
		fclose(fp);
	}
	else
		perror(filename);
	return NULL;
}
