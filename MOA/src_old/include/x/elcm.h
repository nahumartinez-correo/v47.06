global_proc addr shared_address(void);	/* return machine dependant shm addr */
global_proc int read_lan(int fd, char *buffer, size_t bytes, int time_out);
global_proc bool  write_msg(int fd, void *msg, size_t msg_length);


