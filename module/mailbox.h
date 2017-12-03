#ifndef MAILBOX_H
#define MAILBOX_H

#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/fs.h>
#include <linux/spinlock_types.h>

#define ERR_EMPTY -1
#define ERR_FULL -2

union data {
	unsigned int word_count;
	char querty_word[32];
};
struct mailbox_head_t {
	long long int count;
	struct list_head head;
};

struct mailbox_entry_t {
	int who;				//slave =0, master = 1
	char file_path[4096];	//file path
	union data passing;
	struct list_head entry;
};

#endif
