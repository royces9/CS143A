#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	if(argc != 2)
		exit(1);

	//destination file name
	char *dst = argv[1];

	//open flag for write and create
	int open_flag = O_WRONLY | O_CREAT;

	//write and read permissions for user/group/other
	int perm_flag = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	//close stdout
	close(1);

	//open the file from argument list
	int f_dst = open(dst, open_flag, perm_flag);
	if(f_dst == -1) {
		printf("Error opening: %s.\n", dst);
		exit(1);
	}
	
	int pid = fork();
	if(!pid) {
		execv("/bin/ls", argv + 1);
	} else if(pid == -1) {
		printf("Fork error.\n");
	} else {
		wait(&pid);
		close(f_dst);		
	}
	


	exit(0);
}
