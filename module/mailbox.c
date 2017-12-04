#include "mailbox.h"

MODULE_LICENSE("Dual BSD/GPL");	//create driver
static void get_process_name(char *ouput_name);
static ssize_t mailbox_read(struct kobject *kobj,
                            struct kobj_attribute *attr, char *buf);
static ssize_t mailbox_write(struct kobject *kobj,
                             struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobject *hw2_kobject;
static struct kobj_attribute mailbox_attribute
    = __ATTR(mailbox, 0660, mailbox_read, mailbox_write);
// atrribute : create file
static int num_entry_max = 2;

module_param(num_entry_max, int, S_IRUGO);

static void get_process_name(char *ouput_name)
{
	memcpy(ouput_name, current->comm, sizeof(current->comm));
}

static ssize_t mailbox_read(struct kobject *kobj,
                            struct kobj_attribute *attr, char *buf)
{
	if(!HEAD.node_num)
		return ERR_EMPTY;
	else {
		return strlen(buf);	//EMPTY
	}
}
/*
 *add_Node_tail(&HEAD.head)	return(struct mail_entry_t*)
 *remove_Node(struct mail_entry_t *)
 */
static ssize_t mailbox_write(struct kobject *kobj,
                             struct kobj_attribute *attr, const char *buf, size_t count)
{
	struct mail_t* pass = (struct mail_t*)buf;
	printk("path=%s\n",pass->file_path);
	if((num_entry_max - HEAD.node_num)<=1)
		return ERR_FULL;	//FULL
	else {
		struct mailbox_entry_t*	new_node = add_Node_tail(&HEAD.head);
		get_process_name(new_node->who);
		printk("who=%s\n",new_node->who);
		strcpy(new_node->file_path,pass->file_path);
		if(!strcmp(new_node->who,"master"))
			strcpy(new_node->data.query_word,pass->data.query_word);
		if(!strcmp(new_node->who,"slave"))
			new_node->data.word_count = pass->data.word_count;
		//	printk("count=%u\n",new_node->data.word_count);

		return DO;   //DO
	}
}

static int __init mailbox_init(void)
{
	printk("Insert\n");
	hw2_kobject = kobject_create_and_add("hw2", kernel_kobj);
	sysfs_create_file(hw2_kobject, &mailbox_attribute.attr);
	//init HEAD list
	HEAD.node_num = 0;
	HEAD.head.prev = &HEAD.head;
	HEAD.head.next = &HEAD.head;
	return 0;
}

static void __exit mailbox_exit(void)
{
	printk("Remove\n");
	kobject_put(hw2_kobject);
}

module_init(mailbox_init);
module_exit(mailbox_exit);
