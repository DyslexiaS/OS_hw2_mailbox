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
int main(int argc, char **argv)
{
	struct mail_t mail;
	int sysfs_fd = open("/sys/kernel/hw2/mailbox" ,O_RDWR);
	if(sysfs_fd < 0)
	{	
		printf("Fail to open device.\n");
		printf("errno=%d\n",errno);
		return 0;
	}
	int slave_num = 0;
	for(int i=1;i<6;i+=2)
	{
		if(!strcmp("-q",argv[i]))
			strcpy(mail.data.query_word,argv[i+1]);
		else if(!strcmp("-d",argv[i]))
			strcpy(mail.file_path,argv[i+1]);
		else if(!strcmp("-s",argv[i]))
			slave_num = atoi(argv[i+1]);	
		
		else{
			printf("Input error.");
			return 0;
		}
	}
	while(slave_num--)
	{
		int pid = fork();
		if (!pid){
			if(execv("slave",NULL) <0 ){  
	        	perror("error on exec.");  
				exit(0);
			}
		}
	}

	char buf[100]="write";
	int a = write(sysfs_fd,buf,sizeof(buf));
	printf("%d\n",a);
	close(sysfs_fd);
	return 0;

}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	/*
	 * write something or nothing
	 */

	//int ret_val = write(sysfs_fd, ...);
	int ret_val;
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
	return 0;
}
