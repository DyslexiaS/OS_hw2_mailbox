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
#include <linux/slab.h>
#include "def.h"

struct mailbox_head_t {
	long long int node_num;
	struct list_head head;
};
struct mailbox_entry_t {
	char who[20];
	char file_path[4096];	//file path
	union {
		unsigned int word_count;
		char query_word[32];
	} data;
	struct list_head entry;
};
struct mail_t {
	union {
		char query_word[32];
		unsigned int word_count;
	} data;
	char file_path[4096];
};
struct mailbox_head_t HEAD;
struct mailbox_entry_t *add_Node_tail(struct list_head *head)
{
	//struct list_head *listptr;
	//struct mailbox_head_t *get_node = list_entry(listptr, struct mailbox_head_t, head);
	++HEAD.node_num;
	printk("add-->%lld\n",HEAD.node_num);
	struct mailbox_entry_t *new_node;
	new_node=(struct mailbox_entry_t*)kmalloc(sizeof(struct mailbox_entry_t),
	         GFP_KERNEL);
	list_add_tail( &new_node->entry,head);
	return new_node;
}
void remove_Node(struct mailbox_entry_t *del_node)
{
	--HEAD.node_num;
	printk("delete-->%lld\n",HEAD.node_num);
	list_del(&del_node->entry);
	kfree(del_node);
}
#endif
