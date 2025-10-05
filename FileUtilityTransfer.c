#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

char *fifo_path = "/tmp/file_fifo";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_to_send>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    int fd_in = open(filename, O_RDONLY);
    if (fd_in < 0) {
        perror("open input file");
        return 1;
    }

    mkfifo(fifo_path, 0666);

    int fd_fifo = open(fifo_path, O_WRONLY);
    if (fd_fifo < 0) {
        perror("open fifo for writing");
        close(fd_in);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(fd_in, buf, BUF_SIZE)) > 0) {
        write(fd_fifo, buf, n);
    }

    close(fd_in);
    close(fd_fifo);
    printf("File sent successfully.\n");
    return 0;
}
