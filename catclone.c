#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int cat_fd(int fd, const char *filename) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t offset = 0;
        
        while (offset < bytes_read) {
            bytes_written = write(STDOUT_FILENO, buffer + offset, bytes_read - offset);
            if (bytes_written < 0) {
                fprintf(stderr, "cat: write error: %s\n", strerror(errno));
                return -1;
            }
            offset += bytes_written;
        }
    }

    if (bytes_read < 0) {
        fprintf(stderr, "cat: %s: %s\n", filename, strerror(errno));
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int exit_status = 0;

    if (argc == 1) {
        if (cat_fd(STDIN_FILENO, "-") != 0) {
            exit_status = 1;
        }
    } else {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {
                if (cat_fd(STDIN_FILENO, "-") != 0) {
                    exit_status = 1;
                }
            } else {
                int fd = open(argv[i], O_RDONLY);
                if (fd < 0) {
                    fprintf(stderr, "cat: %s: %s\n", argv[i], strerror(errno));
                    exit_status = 1;
                    continue;
                }

                if (cat_fd(fd, argv[i]) != 0) {
                    exit_status = 1;
                }

                close(fd);
            }
        }
    }

    return exit_status;
}
