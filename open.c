
#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main()
{
    int fd, n;
    char buf[3];

    fd = open("test.txt", O_RDONLY);
    printf("%d\n", fd);
    if (fd == -1)
    {
        exit(EXIT_SUCCESS);
    }

    while (n = read(fd, buf, 3))
    {
        printf("%s", buf);
    }
}
