#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


void exec_fun(char *com, char *args[], int *out) {
	int fid[2];

	if( pipe(fid) ) {
		printf("Pipe error.\n");
		exit(1);
	}

	int tempfid = dup(fid[0]);
	if(!out) {
		dup2(fid[1], 1);
	} else {
		dup2(*out, 1);
	}

	int pid = fork();
	if(!pid) {
		execv(com, args);
	} else if(pid == -1) {
		printf("Fork error.\n");
	} else {
		wait(&pid);
	}

	close(fid[1]);
	dup2(tempfid, 0);
}


int main(int argc, char **argv) {
	if(argc != 2)
		exit(1);

	char *dst = argv[1];

	//close stdin/out
	int stdout_copy = dup(1);
	close(0);
	close(1);

	exec_fun("/bin/ls", (char *[]) {"ls", 0}, NULL);
	exec_fun("/bin/grep", (char *[]) {"grep", dst, 0}, NULL);
	exec_fun("/bin/wc", (char *[]) {"wc", 0}, &stdout_copy);

	exit(0);
}
