#ifndef MAIL_H
#define MAIL_H

#include "module/def.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
struct mail_t {
	union {
		char query_word[32];
		unsigned int word_count;
	} data;
	char file_path[4096];
};

int send_to_fd(int sysfs_fd, struct mail_t *mail);
int receive_from_fd(int sysfs_fd, struct mail_t *mail);

#endif
