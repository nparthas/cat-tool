#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// get rid of implicit warning, works without declaration
int fileno(FILE *stream);

void cat(int rfd) {
    int wfd = fileno(stdout);
    static char *buf;
    static size_t bsize;
    struct stat sbuf;

    if (fstat(wfd, &sbuf)) {
        err(1, "stdout");
    }

    if (!buf) {
        bsize = sbuf.st_blksize;
        buf = malloc(bsize);
        if (!buf) {
            err(1, 0);
        }
    }

    ssize_t nr = read(rfd, buf, bsize);
    ssize_t nw;
    while (nr > 0) {
        for (int offset = 0; nr > 0; nr -= nw, offset += nw) {
            nw = write(wfd, buf + offset, nr);
            if (nw < 0) {
                err(1, "stdout");
            }
        }
        nr = read(rfd, buf, bsize);
    }
}

int main(int argc, char *argv[]) {
    // ignore warning
    (void)argc;
    // ignore name
    ++argv;
    int fd = fileno(stdin);
    do {
        if (*argv) {
            if (strcmp(*argv, "-") == 0) {
                fd = fileno(stdin);
            } else {
                fd = open(*argv, O_RDONLY);
            }
            if (fd < 0) {
                err(1, "%s", *argv);
            }
            ++argv;
        }
        cat(fd);
        if (fd != fileno(stdin)) {
            close(fd);
        }
    } while (*argv);
}