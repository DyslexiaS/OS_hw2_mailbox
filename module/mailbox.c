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
		struct mail_t* read_node = (struct mail_t*)buf;
		struct list_head *listptr;
		struct mailbox_entry_t *node;
		char name[50];
		get_process_name(name);
//		printk("%s start to read \n",name);
		list_for_each(listptr, &HEAD.head) {
			node = list_entry(listptr, struct mailbox_entry_t, entry);
			if(!strcmp(name,"master") && !strcmp(node->who,"slave")) {
			printk("(R) name = %s | path = %s | word = %s | count = %u \n", node->who, node->file_path,node->data.query_word,node->data.word_count);
				strcpy(read_node->file_path,node->file_path);
				read_node->data.word_count = node->data.word_count;
				remove_Node(node);
				return SIZE;
			} else if(!strcmp(name,"slave") && !strcmp(node->who, "master")) {
			printk("(R) name = %s | path = %s | word = %s | count = %u \n", node->who, node->file_path,node->data.query_word,node->data.word_count);
				strcpy(read_node->file_path,node->file_path);
				strcpy(read_node->data.query_word, node->data.query_word);
				remove_Node(node);
				return SIZE;
			}
		}
		return ERR_EMPTY;
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
	//printk("path=%s\n",pass->file_path);
	char wname[50];
	get_process_name(wname);
	if(!strcmp(wname,"master")&&(num_entry_max - HEAD.node_num)<=1)
		return ERR_FULL;	//FULL
	else {
		struct mailbox_entry_t*	new_node = add_Node_tail(&HEAD.head);
		get_process_name(new_node->who);
		strcpy(new_node->file_path,pass->file_path);
		if(!strcmp(new_node->who,"master"))
			strcpy(new_node->data.query_word,pass->data.query_word);
		if(!strcmp(new_node->who,"slave"))
			new_node->data.word_count = pass->data.word_count;
		printk("(W) name = %s | path = %s | word = %s | count = %u \n", new_node->who, new_node->file_path, new_node->data.query_word,new_node->data.word_count);
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
