#include "master.h"
int COUNT;
void find_file(int sysfs_fd, struct mail_t* mail)
{
	errno = 0;
	DIR* dir;
	dir = opendir(mail->file_path);
	if (errno == ENOENT)	{
		printf("This direction isn't exist.\n");
		return;
	} else if(errno == ENOTDIR) {	//file
//		printf("file_path=%s\n", mail->file_path);
		++COUNT;
		send_to_fd(sysfs_fd,mail);
	} else {
		struct dirent* ptr;
		while((ptr = readdir(dir)) != NULL) {
			struct mail_t new;
			strcpy(new.data.query_word,mail->data.query_word);
			if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))
				continue;
			else {
				strcpy(new.file_path,mail->file_path);
				strcat(new.file_path,"/");
				strcat(new.file_path,ptr->d_name);
				find_file(sysfs_fd,&new);
			}
		}
		closedir(dir);
	}
}
int main(int argc, char **argv)
{
	struct mail_t mail;
	int sysfs_fd = open("/sys/kernel/hw2/mailbox",O_RDWR);
	if(sysfs_fd < 0) {
		printf("Fail to open device.\n");
		printf("errno=%d\n",errno);
		return 0;
	}
	int slave_num = 0;
	for(int i=1; i<6; i+=2) {
		if(!strcmp("-q",argv[i]))
			strcpy(mail.data.query_word,argv[i+1]);
		else if(!strcmp("-d",argv[i]))
			strcpy(mail.file_path,argv[i+1]);
		else if(!strcmp("-s",argv[i]))
			slave_num = atoi(argv[i+1]);
		else {
			printf("Input error.");
			return 0;
		}
	}
	while(slave_num--) {
		int pid = fork();
		if (!pid) {
			if(execv("slave",NULL) <0 ) {
				perror("error on exec.");
				exit(0);
			}
		}
	}
	//FIND FILE
	printf("%s\n",mail.file_path);
	if(mail.file_path[strlen(mail.file_path)-1] == '/')
		mail.file_path[strlen(mail.file_path)-1] = '\0';
	find_file(sysfs_fd,&mail);

	while(1){
		struct mail_t result ;
		receive_from_fd(sysfs_fd, &result);
		if(!COUNT){
			kill(0,2);
			close(sysfs_fd);
		}
	}
	return 0;

}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	while(1) {
		lseek(sysfs_fd,0,SEEK_SET);
		int ret_val = write(sysfs_fd,(char*)mail,sizeof(*mail));
		if (ret_val == ERR_FULL) {
			struct mail_t result ;
			receive_from_fd(sysfs_fd, &result);
			//return 0;
		} else if(ret_val == DO) {
		//	printf("master write successfully.%d\n",ret_val);
			return 0;
		} else {
			printf("master write Error. ret=%d\n",ret_val);
			exit(-1);
		}
	}
}
int receive_from_fd(int sysfs_fd, struct mail_t *result)
{
	while(1) {
		lseek(sysfs_fd,0,SEEK_SET);
		int ret_val = read(sysfs_fd, (char*)result, sizeof(*result));
		if (ret_val == ERR_EMPTY) {
			continue;
		} else if(ret_val == SIZE) {
			--COUNT;
			printf("FILE_PATH = %s\n", result->file_path);
			printf("WORD_COUNT = %u\n", result->data.word_count);
			return 0;
		} else {
			printf("master read Error. ret=%d\n",ret_val);
			exit(-1);
		}
	}
}
