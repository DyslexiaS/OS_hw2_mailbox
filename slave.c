#include "slave.h"

int main(int argc, char **argv)
{
	/*
	 * write something or nothing
	 */
	printf("I'm slave");
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

	//int ret_val = read(sysfs_fd, ...);
	int ret_val;
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
}
