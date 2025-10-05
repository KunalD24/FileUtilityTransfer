#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

char *fifo_path = "/tmp/file_fifo";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <output_file>\n", argv[0]);
        return 1;
    }

    char *outfile = argv[1];
    mkfifo(fifo_path, 0666);

    int fd_fifo = open(fifo_path, O_RDONLY);
    if (fd_fifo < 0) {
        perror("open fifo for reading");
        return 1;
    }

    int fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd_out < 0) {
        perror("open output file");
        close(fd_fifo);
        return 1;
    }

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(fd_fifo, buf, BUF_SIZE)) > 0) {
        write(fd_out, buf, n);
    }

    close(fd_out);
    close(fd_fifo);
    printf("File received successfully.\n");
    return 0;
}
