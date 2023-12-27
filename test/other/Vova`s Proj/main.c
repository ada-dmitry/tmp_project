#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

int linesWritten = 0;

void signalHandler(int signal)
{
    printf("\b\bNumber of lines written: %d\n", linesWritten);
    fflush(stdout);
}

void writeFile(const char *fileName, int numLines, const char *placeholder)
{
    int fd = open(fileName, O_CREAT | O_WRONLY, 0666);

    if (fd == -1)
    {
        printf("Failed to open the file\n");
        exit(EXIT_FAILURE);
    }

    if (flock(fd, LOCK_EX) == -1)
    {
        printf("Failed to lock file for exclusive use\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, 0) == -1)
    {
        printf("Failed to truncate the file\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, signalHandler);

    for (int i = 0; i < numLines; i++)
    {
        dprintf(fd, "%s\n", placeholder);
        linesWritten++;

        usleep(100000);
    }

    if (flock(fd, LOCK_UN) == -1)
    {
        printf("Failed to unlock file\n");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <filename> <number of lines> <placeholder>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *fileName = argv[1];
    if (strstr(fileName, ".txt") == NULL)
    {
        printf("The provided file must be ended with .txt\n");
        exit(EXIT_FAILURE);
    }

    int numLines = atoi(argv[2]);
    if (numLines <= 0)
    {
        printf("The number of lines must be a positive integer\n");
        exit(EXIT_FAILURE);
    }

    const char *placeholder = argv[3];

    writeFile(fileName, numLines, placeholder);

    return 0;
}