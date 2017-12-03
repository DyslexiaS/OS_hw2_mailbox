#include<stdio.h>
#include<errno.h>
#include<dirent.h>
#include<string.h>
void find_file(char* path){
	errno = 0;
	DIR* dir;
	dir = opendir(path);
	if (errno == ENOENT)	{
		printf("This direction isn't exist.\n");
		return;
	} else if(errno == ENOTDIR) {	//file
		printf("file_path=%s\n", path);
	} else {
		struct dirent* ptr;
		while((ptr = readdir(dir)) != NULL) {
			//printf("new_path=%s dname=%s\n", path,ptr->d_name);
			char new_path[4096]="";
//			if(ptr->d_name[0] == '.')
			if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))
				continue;
			else {
				strcpy(new_path,path);
				strcat(new_path,"/");
				strcat(new_path,ptr->d_name);
				find_file(new_path);
			}
		}
		closedir(dir);
	}
}
int main(int argc,char** argv){
	char path[4096] = "";
	strcpy(path,argv[1]);
	find_file(path);
	return 0;
}
