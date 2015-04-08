#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>

#define BUFSIZE 4096
#define NAMESIZE 256

int main(int argc, char *argv[])
{
        int file;
        int file_out;

        int rf_size;
        int wf_size;

        int i,j;

        ssize_t nread;

        char readf[NAMESIZE];
        char writef[NAMESIZE];
        char buffer[BUFSIZE];

        int flags = O_WRONLY | O_CREAT | O_TRUNC;
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	if(argc < 3)
	{
		write(1,"Original File : ",16);
		if(read(0,readf,NAMESIZE-1)<0)
		{
		        write(1,"Error(1)\n",10);
		        return 1;
		}

		i = 0;
		while(1)
		{
		        if(readf[i] == '\n' || readf[i] == 0)
		        {
		                readf[i] = 0;
		                break;
		        }
		        i++;
		}
		rf_size = i;    

		write(1, "Target File : ",14);

		if(read(0,writef,NAMESIZE-1)<0)
		{
		        write(1,"Error(2)\n",10);
		        return 1;
		}

		j = 0;
		while(1)
		{
		        if(writef[j] == '\n' || writef[j] == 0)
		        {
		                writef[j] = 0;
		                break;
		        }
		        j++;
		}
		rf_size = j;
	}
	else
	{
		strncpy(readf,argv[1],strlen(argv[1]));
		strncpy(writef,argv[2],strlen(argv[2]));
	}
        

        if((file = open(readf, O_RDONLY)) == -1 )
        {
                write(1, "fail(1)\n", 9);
                return 1;
        }

        if((file_out = open(writef, flags, mode)) == -1)
        {
                write(1,"fail(2)\n", 9);
                close(file);
                return 1;
        }


        while( (nread = read(file, buffer, BUFSIZE)) > 0 )
        {
                if(write(file_out, buffer, nread) < nread)
                {
                        close(file);
                        close(file_out);
                        return 1;
                }
        }

				close(file);
        close(file_out);
        write(1, "Complete\n",10);
        
        return 0;
}

        
