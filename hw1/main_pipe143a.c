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

	//save read end of pipe
	int tempfid = dup(fid[0]);

	//set write end to 1, or if something else is declared
	if(!out) {
		out = &fid[1];
	}

	dup2(*out, 1);

	int pid = fork();
	if(!pid) {
		execvp(com, args);
	} else if(pid == -1) {
		printf("Fork error.\n");
	} else {
		wait(&pid);
	}

	//close the write end
	close(fid[1]);

	//set the read end to 0
	dup2(tempfid, 0);
}


int main(int argc, char **argv) {
#if 0
	if(argc != 2)
		exit(1);

	char *dst = argv[1];
#else
	char *dst = "main";
#endif
	//close stdin/out
	int stdout_copy = dup(1);
	close(0);
	close(1);

	/*
	exec_fun("ls", (char *[]) {"ls", 0}, NULL);
	exec_fun("grep", (char *[]) {"grep", dst, 0}, NULL);
	exec_fun("wc", (char *[]) {"wc", 0}, &stdout_copy);
*/
	char *ls = "ls";
	char *grep = "grep";
	char *wc = "wc";

	char *lss[] = {ls, NULL};
	char *grepp[] = {grep, dst, NULL};
	char *wcc[] = {wc, NULL};

	char **test[] = {lss, grepp, wcc};
	//char **test[] = {(char *[]) {"ls", NULL}, (char *[]) {"grep", dst, NULL}, (char *[]) {"wc", 0}};
	int *outs[] = {NULL, NULL, &stdout_copy};

	for(int i = 0; i < 3; ++i)
		exec_fun(test[i][0], test[i], outs[i]);

	exit(0);
}
