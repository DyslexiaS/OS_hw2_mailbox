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
	printf("I'm slave\n");
	char str[1000] = "";
	while(1) {
		struct mail_t mail;
		receive_from_fd(sysfs_fd, &mail);
		printf("slave open %s\n",mail.file_path);
		mail.data.word_count = 0;
		FILE * pf = fopen(mail.file_path, "r");
		while(fscanf(pf, mail.file_path, str)!=EOF) {
			mail.data.word_count += compare(mail.data.query_word,str);
		}
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
		} else if(ret_val == SIZE) {
			return 0;
		} else {
			printf("slave read Error.\n");
			exit(-1);
		}
	}
}
