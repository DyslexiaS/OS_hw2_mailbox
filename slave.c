#include "slave.h"
int compare(char*find, char*str)
{
	int len = strlen(find);
	int len2 = strlen(str);
	len = len<=len2 ? len :len2;
	for(int i=0; i<len; ++i) {
		find[i] |= 1<<5;
		str[i] |= 1<<5;
	}
	if( !strcmp(find,str))	return 1;
	else	return 0;
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
	printf("I'm slave\n");
	char str[1000] = "";
	while(1) {
		receive_from_fd(sysfs_fd, &mail);
		FILE * pf = fopen(mail.file_path, "r");
		while(fscanf(pf, mail.file_path, str)!=EOF) {
			mail.data.word_count += compare(mail.data.query_word,str);
		}
		// Work de done

		send_to_fd(sysfs_fd, &mail);
	}
}

int send_to_fd(int sysfs_fd, struct mail_t *mail)
{
	while(1) {
		int ret_val = write(sysfs_fd,(char*)mail, sizeof(*mail));
		if (ret_val == ERR_FULL) {
			continue;
		} else if(ret_val==DO) {
			return 0;
		} else {
			printf("Error.\n");
			exit(-1);
		}
	}
}
int receive_from_fd(int sysfs_fd, struct mail_t *mail)
{
	while(1) {
		int ret_val = read(sysfs_fd,(char*)mail,sizeof(*mail));
		if (ret_val == ERR_EMPTY) {
			continue;
		} else if(ret_val == DO) {
			return 0;
		} else {
			printf("Error.\n");
			exit(-1);
		}
	}
}
