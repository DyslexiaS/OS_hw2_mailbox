#include "slave.h"
int compare(char*find, char*str)
{
	int len = strlen(find);
	int len2 = strlen(str);
	if(len2-len == 0 ||(len2-len == 1 && str[len2-1] == '.')) {
		for(int i=0; i<len; ++i) {
			find[i] |= 1<<5;
			str[i] |= 1<<5;
		}
		if( !strncmp(find,str,len)) return 1;
		else    return 0;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int sysfs_fd = open("/sys/kernel/hw2/mailbox",O_RDWR);
	if(sysfs_fd < 0) {
		printf("Fail to open device.\n");
		printf("errno=%d\n",errno);
		return 0;
	}
	char str[1000] = "";
	char find[32] = "";
	while(1) {
		struct mail_t mail;
		receive_from_fd(sysfs_fd, &mail);
		strcpy(find, mail.data.query_word);
		//	printf("slave open: path = %s| find word = %s \n",mail.file_path,mail.data.query_word);
		mail.data.word_count = 0;
		FILE * pf = fopen(mail.file_path, "r");
		while(fscanf(pf, "%s", str)!=EOF) {
			mail.data.word_count += compare(find,str);
		}
		fclose(pf);
		// Work  done
		send_to_fd(sysfs_fd, &mail);
	}
}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	while(1) {
		lseek(sysfs_fd,0,SEEK_SET);
		int ret_val = write(sysfs_fd,(char*)mail, sizeof(*mail));
		if (ret_val == ERR_FULL) {
//			printf("salve write FULL.\n");
			continue;
		} else if(ret_val==DO) {
			//		printf("salve write successfully.\n");
			return 0;
		} else {
			printf("salve write Error.\n");
			exit(-1);
		}
	}
}
int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	while(1) {
		lseek(sysfs_fd,0,SEEK_SET);
		int ret_val = read(sysfs_fd,(char*)mail,sizeof(*mail));
		if (ret_val == ERR_EMPTY) {
//			printf("salve read EMPTY.\n");
			continue;
		} else if(ret_val == SIZEQQ) {
			//		printf("salve read successfully.\n");
			return 0;
		} else {
			printf("size%d\n",SIZEQQ);
			printf("slave read Error.%d\n",ret_val);
			exit(-1);
		}
	}
}
