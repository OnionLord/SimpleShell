#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <term.h>
#include <grp.h>
#include <pwd.h>

#define PATH_LENGTH 	200

//�ɼǿ����� ���� ����
#define FLAG_ALL 	0x01
#define FLAG_INODE 	0x02
#define FLAG_LONG 	0x04
#define FLAG_RECUR	0x08
#define FILEINFO 	struct file_info

//���ϱ��̸� ����
#define FILENAME_SIZE	256

#define EXTENDCHAR	5

struct file_info{
	char filename[FILENAME_SIZE+1];
	unsigned long inode;
	unsigned long mode;
	unsigned long nlink;
	unsigned long uid;
	unsigned long gid;
	unsigned long size;
	//������ �����ð� ���
	time_t atime;  
	//�� ������ �����ϴ� block��
	unsigned long blocks;
	struct file_info *next;
};


void seek_dir(char *dir,int opt);																// ���丮�� ��ȸ�ϴ� �Լ�
void print_long(FILEINFO *list_head,int opt);													// -l�ɼ��� ������ ȭ�鿡 ���������� �����
void save_long(FILEINFO **list_head,struct stat *cur_stat,struct dirent *cur_dir,int opt);		// -l�ɼ��� ������ ���������� ��ũ�帮��Ʈ�� �����ϴ� �Լ�
void print_normal(FILEINFO *list_head,int opt);													// -l �ɼ��� ������ ȭ�鿡 ���������� ����ϴ� �Լ�
void save_normal(FILEINFO **list_head,struct stat *cur_stat,struct dirent *cur_dir,int opt);	// -l �ɼ��� ������ ���������� ��ũ�帮��Ʈ�� �����ϴ� �Լ�
void free_list(FILEINFO *list_head);															// ��ũ�帮��Ʈ�� �޸𸮸� �����ϴ� �Լ�
void sort_list(FILEINFO *list_head);															// ��ũ�帮��Ʈ�� ����� �ڷḦ �����̸������� �����ϴ� �Լ�
int long_len(unsigned long num);																// unsigned long ���� �Է¹޾� �ڸ����� �����ϴ� �Լ�
char* uid_to_name(uid_t);								
char* gid_to_name(gid_t);
int SizeSum(char filepath[]);																	// Path�� �Է��� �� ���丮�� ũ�⸦ ���ϴ� �Լ�
char * ReadPathAndFilenameCpy(char * dirname, char * filename);									// Path�� �а� �����޸� �Ҵ��� �ϰ� �����ϴ� �Լ�
int ExtenMem(char ** str, int maxStringLen);													// �Է¹��� ���ڰ� ū ��� �����޸� ũ�⸦ Ȯ���ϴ� �Լ�

int main(int argc,char* argv[])
{
	int i, j;
	int opt=0;						//�ɼ��� �����ϴ� ����
	char path[PATH_LENGTH]=".";	// ����� path�ɼ��� ���־������� ����Ʈ ��ΰ�
	
	
	for(i=1;i<argc;i++)
	{
		for(j=1;argv[i][j] != '\0';j++)
		{
			if(argv[i][0] != '-')	//�ɼ��� �ƴϰ� path �����϶�
			{
				strncpy(path,argv[i],PATH_LENGTH-1);
				break;
			}
			switch(argv[i][j])	//���� ls������ �� �ɼ��� ��ҹ��ڸ� ������
			{
			case 'a':
				opt |= FLAG_ALL;
				break;
			case 'l':
				opt |= FLAG_LONG;
				break;
			case 'i':
				opt |= FLAG_INODE;
				break;
			case 'R':	
				opt |= FLAG_RECUR;
				break;
			}
		}
	}
	
	if(path[strlen(path)-1] != '/')
	{
	//	sprintf(path+strlen(path),"/");
		path[strlen(path)]='/';
	}
	printf("%c[1;31m",27);  //������
	//printf("\ndirectory path:");
	seek_dir(path,opt);		//���丮�� ��ȸ��
	
	
	printf("%c[0m\n",27);  //������
	return 0;
}

int long_len(unsigned long num)	//ȭ�鿡 �����Ͽ� ����ϱ����Ͽ� ĭ���� ���
{
	int nCount=0;
	while( num%10 || num/10 ){
		num /=10;
		nCount++;
	}
	
	return nCount;
}

void free_list(FILEINFO *list_head){
	FILEINFO *tmp_list;
	
	while(list_head != NULL){     //�޸� �����ϱ�
		tmp_list = list_head;
		list_head = list_head->next;
		free(tmp_list);
	}
}

//��ũ�帮��Ʈ�� ����ִ��ڷḦ �����̸������� ����
void sort_list(FILEINFO *list_head){	
	FILEINFO *tmp_list_left;
	FILEINFO *tmp_list_right;
	
	FILEINFO tmp_list;
	FILEINFO *tmp_listp;
	
	
	if( list_head == NULL )
		return ;
	
	if( list_head->next == NULL)
		return ;
	
	tmp_list_left = list_head;
	tmp_list_right = list_head->next;
	
	
	while( tmp_list_left->next != NULL){
		
		while(tmp_list_right != NULL){
			
			//�ΰ��� ����Ʈ�� ���� exchange�ϴ� �ڵ�
			if( strcmp(tmp_list_left->filename,tmp_list_right->filename) >0 ){	
				memcpy(&tmp_list , tmp_list_left,sizeof(FILEINFO));
				memcpy(tmp_list_left , tmp_list_right,sizeof(FILEINFO));
				memcpy(tmp_list_right , &tmp_list,sizeof(FILEINFO));
				tmp_listp = tmp_list_left->next;
				tmp_list_left->next = tmp_list_right->next;
				tmp_list_right->next = tmp_listp;
			}
			
			tmp_list_right = tmp_list_right->next;
		}
		tmp_list_left = tmp_list_left->next;
		tmp_list_right = tmp_list_left->next;
	}
	
}

void seek_dir(char *dir,int opt)
{
	DIR *dp;
	struct dirent *entry;
	struct stat tmp_stat;
	FILEINFO *tmp_list;
	FILEINFO *list_head;
	
	list_head = NULL;
	
	printf("%c[1;31m",27);	//������
	printf("%s\n",dir);	//��θ� ���
	printf("%c[1;0m",27); //���� ����

	if((dp = opendir(dir)) == NULL){	//���丮 ��Ʈ���� Open��
		fprintf(stderr,"directory open error: %s\n",dir);
		return;
	}
	
	chdir(dir);
	
	while((entry = readdir(dp)) != NULL){	//���丮������ �Ѱ��� �о��
		lstat(entry->d_name, &tmp_stat);
		if( opt & FLAG_LONG)	//�ɼǿ� ���� ���������� �����ϴ� ����� �޸���
			save_long(&list_head,&tmp_stat,entry,opt);
		else
			save_normal(&list_head,&tmp_stat,entry,opt);
	}
	
	sort_list(list_head);	//�о�� �ڷḦ ������
	
	
	if (opt & FLAG_LONG) {	//�ɼǿ� ���� ���������� ����ϴ� ����� �޸���
		printf("total %d\n", SizeSum(dir));
		print_long(list_head, opt);
	}
	else
		print_normal(list_head,opt);
	
	
	tmp_list = list_head;
	
	
	while( tmp_list != NULL) {
		if(S_ISDIR(tmp_list->mode)){
			
			// '.' �� '..'�� ��� ���� ������
			if( strcmp(".",tmp_list->filename) == 0 || strcmp("..",tmp_list->filename) == 0 ){
				tmp_list = tmp_list->next;
				continue;
			}
			if( opt & FLAG_RECUR){          //��� �ɼ��� �������� ����
				
				printf("%c[1;31m",27);  //������
				printf("\n\n%s",dir);
				
				seek_dir(tmp_list->filename,opt);
			}
		}
		
		tmp_list = tmp_list->next;
		
	}
	chdir("..");
	
	closedir(dp);
	free_list(list_head);	//���������� ����� �޸𸮸� ������
}
void save_long(FILEINFO **list_head,struct stat *cur_stat,struct dirent *cur_dir,int opt)
{
	FILEINFO *cur_list=(*list_head);
	
	if( *list_head != NULL)
		while( cur_list->next != NULL)
			cur_list = cur_list->next;
        
        if( cur_dir->d_name[0] == '.' )
			if( !(opt & FLAG_ALL) )
				return; //all�ɼ��� ������ .���� �����ϴ� ������ ��ŵ����. 
			
			
			if( (*list_head) == NULL){   //����Ʈ�� ��ó���϶�
                cur_list = (FILEINFO *)malloc(sizeof(FILEINFO));
                cur_list->next = NULL;
                *list_head = cur_list;
			}else{
                cur_list->next = (FILEINFO *)malloc(sizeof(FILEINFO));
				cur_list = cur_list->next;
                cur_list->next = NULL;
			}
			
			
			cur_list->inode = cur_stat->st_ino;
			cur_list->mode = cur_stat->st_mode;
			strcpy(cur_list->filename ,cur_dir->d_name);
			
			//hard-link ��
			cur_list->nlink = cur_stat->st_nlink;
			
			//User ID  
			cur_list->uid = cur_stat->st_uid;
			
			//Group ID
			cur_list->gid = cur_stat->st_gid;
			
			//File Size 
			cur_list->size = cur_stat->st_size;
			
			//������ �����ð� 
			cur_list->atime = cur_stat->st_atime;   //�ð����� �м��Ͽ� ����ü�� ����
			
			//�� ������ �����ϴ� block��
			cur_list->blocks = cur_stat->st_blocks;
			
}               

void print_long(FILEINFO *list_head,int opt)
{
	FILEINFO *cur_list;
	unsigned long tmp_perm;
	struct tm *tm_ptr;
	int i;

	cur_list = list_head;
	
	while(cur_list != NULL){
		tmp_perm = cur_list->mode;
		
		if(opt & FLAG_INODE)
			printf("%u ",(unsigned int)cur_list->inode);	
		
		//Permission ���
		for(i=0;i<3;i++)
		{
			if(tmp_perm & S_IRUSR)
				printf("r");
			else
				printf("-");
			
			if(tmp_perm & S_IWUSR)
				printf("w");
			else
				printf("-");
			
			if(tmp_perm & S_IXUSR)
				printf("x");
			else
				printf("-");
			
			tmp_perm <<=3;
		}
		
		//hard-link �� ���
		printf(" %2u",(unsigned int)cur_list->nlink);
		
		//User ID���
		printf(" %-8s",uid_to_name(cur_list->uid));
		
		//Group ID���
		printf(" %-8s",gid_to_name(cur_list->gid));
		
		//File Size ���
		printf(" %12u",(unsigned int)cur_list->size);
		
		//������ �����ð� ���
		tm_ptr = gmtime(&cur_list->atime);	//�ð����� �м��Ͽ� ����ü�� ����
		printf(" %02d/%02d/%2d %02d:%02d",tm_ptr->tm_year%100,tm_ptr->tm_mon,tm_ptr->tm_mday,tm_ptr->tm_hour,tm_ptr->tm_min);
		
		//�� ������ �����ϴ� block��
		printf("%5u",(unsigned int)cur_list->blocks);
				
		//File Type������ ����� ���� �޸���
		if(S_ISREG(cur_list->mode)){
			if(cur_list->mode & 01001001)	//������������ �˻�
				printf("%c[1;32m",27);
			else
				printf("%c[0m",27);
			//printf("REG  ");
		}else if(S_ISDIR(cur_list->mode)){
			printf("%c[1;34m",27);
			//printf("DIR  ");
		}else if(S_ISCHR(cur_list->mode)){
			printf("%c[1;37m",27);
			//printf("CHR  ");
		}else if(S_ISBLK(cur_list->mode)){
			printf("%c[1;33m",27);
			//printf("BLK  ");
		}else if(S_ISFIFO(cur_list->mode)){
			printf("%c[1;37m",27);		
			//printf("FIFO ");
		}else if(S_ISLNK(cur_list->mode)){
			printf("%c[1;36m",27);
			//printf("LNK  ");
		}else if(S_ISSOCK(cur_list->mode)){
			printf("%c[1;35m",27);
			//printf("SOCK ");
		}
		
		//30->�⺻�� ���� ȸ��
		//31->���� ����
		//32->�������� ���
		//33-> ���
		//34->���丮 �����Ķ�
		//35->������
		//36-> ���� CYAN ��ũ
		//37->���� ���

		//�����̸� ���
		printf(" %s",cur_list->filename);
		
		//���� ����
		printf("%c[1;0m",27);

		printf("\n");
		cur_list = cur_list->next;
	}
}

void save_normal(FILEINFO **list_head,struct stat *cur_stat,struct dirent *cur_dir,int opt)
{
	FILEINFO *cur_list=*list_head;
	
	if( *list_head != NULL)
		while( cur_list->next != NULL)
			cur_list = cur_list->next;
		
		
		if( cur_dir->d_name[0] == '.')
			if( !(opt & FLAG_ALL) )
				return; //all�ɼ��� ������ .�� ..�� ��ŵ�Ѵ�.
			
			
			if( (*list_head) == NULL){	//����Ʈ�� ��ó���϶�
				cur_list = (FILEINFO *)malloc(sizeof(FILEINFO));
				cur_list->next = NULL;
				*list_head = cur_list;
			}else{
				cur_list->next = (FILEINFO *)malloc(sizeof(FILEINFO));
				cur_list = cur_list->next;
				cur_list->next = NULL;
			}
			
			cur_list->inode	= cur_stat->st_ino;
			cur_list->mode = cur_stat->st_mode;
			strcpy(cur_list->filename ,cur_dir->d_name);
			
			
}

void print_normal(FILEINFO *list_head,int opt)
{
	FILEINFO *cur_list;
	int i,j;
	int col_length_name[15];	//���������� ĭ �ʺ� �����ϴ� �迭
	int col_length_inode[15];
	int col_num=0;
	int nCount;
	int index;
	int term_x,term_y;	//�͹̳��� ũ�⸦ �о���� �Լ�
	
	for(i=0;i<15;i++){
		col_length_name[i] = 0;
		col_length_inode[i] = 0;
	}
	
	
	//�͹̳� ȭ���� ũ�� ������ ����
	setupterm(NULL,fileno(stdout),(int *)0);
	term_y = tigetnum("lines");	//���� �͹̳��� ũ�⸦ ���´�.
	term_x = tigetnum("cols");
	
	
	for(j=15;j>0;j--){
		cur_list = list_head;
		index  =0;
		nCount = 0;
		
		//���� ���� ��½� ���������� ���� ������ ĭ �ʺ� ����ϴ�  �ڵ�κ�
		while(cur_list != NULL){	
			if( col_length_name[index%j] < strlen(cur_list->filename) )
				col_length_name[index%j] = strlen(cur_list->filename);
			
			if( opt & FLAG_INODE){
				if( col_length_inode[index%j] < long_len(cur_list->inode) )
					col_length_inode[index%j] =long_len(cur_list->inode);
			}
			
			nCount = 0;
			for(i=0;i<j;i++){
				nCount += col_length_name[i]+2;
				if( opt & FLAG_INODE)
					nCount += col_length_inode[i]+1;
			}
			
			if(term_x < nCount)	
				break;
			
			nCount = 0;

			index ++;
			index = index %j;
			cur_list = cur_list->next;
		}
		if(cur_list == NULL){
			col_num = j;
			break;
		}

	}
	
	cur_list = list_head;
	index = 0;
	
	while(cur_list != NULL){
		if(S_ISREG(cur_list->mode)){
			if(cur_list->mode & 01001001) //������������ �˻�
				printf("%c[1;32m",27);
			else
				printf("%c[0m",27);
			
		}else if(S_ISDIR(cur_list->mode)){
			printf("%c[1;34m",27);
		}else if(S_ISCHR(cur_list->mode)){
			printf("%c[1;37m",27);
		}else if(S_ISBLK(cur_list->mode)){
			printf("%c[1;33m",27);
		}else if(S_ISFIFO(cur_list->mode)){
			printf("%c[1;37m",27);
		}else if(S_ISLNK(cur_list->mode)){
			printf("%c[1;36m",27);
		}else if(S_ISSOCK(cur_list->mode)){ 
			printf("%c[1;35m",27);
		}
		
		if( opt & FLAG_INODE )
			printf(" %*u",col_length_inode[index],(unsigned int)cur_list->inode);
		
		printf(" %*s ",col_length_name[index],cur_list->filename);
		cur_list = cur_list->next;
		if( index == col_num -1)
			printf("\n");
		index ++;
		index = index % col_num;
		
	}
}

#include <pwd.h>
char *uid_to_name(uid_t uid)
{
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(numstr, "d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

#include <grp.h>
char *gid_to_name(gid_t gid)
{
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];
	if ((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

int SizeSum(char filepath[]) //�հ� ���ϴ� �Լ� >> ���� ���׸����̼� ���� ����. 
{
	struct dirent * entry;
	DIR * dirpt;
	int sum = 0;
	struct stat buf;
	char * filename;

	dirpt = opendir(filepath);

	while ((entry = readdir(dirpt)) != '\0')
	{
		filename = ReadPathAndFilenameCpy(filepath, entry->d_name);
		if ((lstat(filename, &buf)) == 0 && entry->d_name[0] != '.')  //lstat �����ø� ��� for check
			sum = sum + buf.st_blocks;
	}
	free(filename);
	closedir(dirpt);

	return sum / 2;
}

char * ReadPathAndFilenameCpy(char * dirname, char * filename)
{
	int maxPathLen = PATH_LENGTH;
	char * path = (char *)malloc(sizeof(char)*maxPathLen);
	int idx = 0, idx2 = 0;

	while (1)
	{
		if (idx >= maxPathLen)
			maxPathLen = ExtenMem(&path, maxPathLen);
		if (dirname[idx] == '\0')
			break;
		path[idx] = dirname[idx];
		idx++;
	}

	if (idx >= maxPathLen)
		maxPathLen = ExtenMem(&path, maxPathLen);

	path[idx++] = '/';

	while (1)
	{
		if (idx >= maxPathLen)
			maxPathLen = ExtenMem(&path, maxPathLen);
		if (filename[idx2] == '\0')
		{
			path[idx] = '\0';
			break;
		}
		path[idx] = filename[idx2];
		idx++, idx2++;
	}
	return path;
}

int ExtenMem(char ** str, int maxStringLen)
{
	char * newStr;
	int i;
	newStr = (char *)malloc(sizeof(char)*(EXTENDCHAR + maxStringLen));

	for (i = 0; i<maxStringLen; i++)
		newStr[i] = (*str)[i];

	free(*str);
	*str = newStr;

	return maxStringLen += EXTENDCHAR;
}