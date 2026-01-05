#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
	int fd = open("/dev/pico_sysmon", O_RDWR);
	if (fd < 0) { perror("open"); return 1; }

	if (argc > 1) {
		const char* cmd = argv[1];
		if (write(fd, cmd, strlen(cmd)) < 0) { perror("write"); close(fd); return 1; }
		printf("Sent: %s\n", cmd);
	}

	char buf[512];
	memset(buf, 0, sizeof(buf));
	ssize_t n = read(fd, buf, sizeof(buf)-1);
	if (n < 0) { perror("read"); close(fd); return 1; }

	printf("%s", buf);
	close(fd);
	return 0;
}
