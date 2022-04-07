#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Usage: %s pid\n",argv[0]);
		return 0;
	}
	int pid = 0, n = 0;
	n = sscanf(argv[1],"%d", &pid);
	if(n<1)
	{
		printf("Bad pid!🌚🌚\n");
		return 0;
	}
	char path[1024] = {0};
	char fullpath[1024] = {0};
	char targetname[128] = {0};
	sprintf(path,"/proc/%d/map_files",pid);
	sprintf(targetname,"%d.dump",pid);
	struct dirent **dir = 0;
	n = scandir(path,&dir,0,alphasort);
	if(n<1)
        {
                printf("Not Found this pid!🌚🌚\n");
                return 0;
        }
	FILE *read, *write;
	unsigned long long startaddr = 0, endaddr = 0, len = 0;
	write = fopen(targetname,"wb");
    	for(int i=0;i<n;i++)
	{
		sprintf(fullpath,"%s/%s",path,dir[i]->d_name);
		sscanf(dir[i]->d_name,"%llx-%llx", &startaddr, &endaddr);
		//printf("start: %llx, end: %llx\n", startaddr, endaddr);
		len = endaddr - startaddr;
		unsigned char *buf = (unsigned char *)malloc(len);
		read = fopen(fullpath,"rb");
		if(read)
		{
			fread(buf,1,len,read);
			fclose(read);
			fwrite(buf,1,len,write);
		}
		else
		{
			printf("Write zeros...\n");
			read = fopen("/dev/zero","rb");
			fread(buf,1,len,read);
			fclose(read);
                        fwrite(buf,1,len,write);
		}
		free(buf);
	}
		
    return(0);     
}
