
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define SERVER_KEY 0x1aaaaaa4
#define REQ_MSG_SIZE 8192
#define RESP_MSG_SIZE 8192

struct requestMsg
{
    long mtype;
    int clientId; /* ID of client's message queue */
    char pathname[REQ_MSG_SIZE];
};

struct responseMsg
{
    long mtype;
    char data[RESP_MSG_SIZE];
};

#define RESP_MT_FAILURE 1 /* File couldn't be opened */
#define RESP_MT_DATA 2    /* Message contains file data */
#define RESP_MT_END 3     /* File data complete */
