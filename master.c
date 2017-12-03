#include "master.h"
/* mail.h
   struct mail_t {
   union {
   char query_word[32];
   unsigned int word_count;
   } data;
   char file_path[4096];
   };
   int send_to_fd(int sysfs_fd, struct mail_t *mail);
   int receive_from_fd(int sysfs_fd, struct mail_t *mail);
   */
void find_file(int sysfs_fd, struct mail_t* mail)
{
	errno = 0;
	DIR* dir;
	dir = opendir(mail->file_path);
	if (errno == ENOENT)	{
		printf("This direction isn't exist.\n");
		return;
	} else if(errno == ENOTDIR) {	//file
		printf("file_path=%s\n", mail->file_path);
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
	find_file(sysfs_fd,&mail);
	close(sysfs_fd);
	return 0;

}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{

	int ret_val = write(sysfs_fd,mail->file_path,sizeof(mail->file_path));
	printf("ret=%d\n",ret_val);
	if (ret_val == ERR_FULL) {
		/*
		 * write something or nothing
		 */
	} else {
		/*
		 * write something or nothing
		 */
	}

	/*
	 * write something or nothing
	 */
}

int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	/*
	 * write something or nothing
	 */

	int ret_val;
	//int ret_val = read(sysfs_fd, ...);
	if (ret_val == ERR_EMPTY) {
		/*
		 * END
		 */
	} else {
		/*
		 * write something or nothing
		 */
	}

	/*
	 * write something or nothing
	 */
	return 0;
}
