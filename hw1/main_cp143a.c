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
	if(argc != 3)
		exit(1);

	char *src = argv[1];
	char *dst = argv[2];

	if(!strcmp(src, dst))
		exit(1);

	int f_src = open(src, O_RDONLY);
	if(f_src == -1) {
		printf("Error opening: %s.\n", src);
		exit(1);
	}

	int open_flag = O_WRONLY | O_CREAT;
	int perm_flag = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	int f_dst = open(dst, open_flag, perm_flag);
	if(f_dst == -1) {
		printf("Error opening: %s.\n", dst);
		exit(1);
	}

	int s_buf = 4096;
	char buf[s_buf];
	int size = 0;
	while( (size = read(f_src, buf, s_buf)) )
		write(f_dst, buf, size);

	close(f_src);
	close(f_dst);

	exit(0);
}
