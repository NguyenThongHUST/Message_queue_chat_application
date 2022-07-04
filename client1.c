
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct message
{
    long type;
    char text[200];
};

struct message requestMsg;
struct message respondMsg;

void main()
{
    int msqid1, msqid2, len;
    key_t key1 = 2102;
    key_t key2 = 3102;

    if ((msqid1 = msgget(key1, 0644)) == -1)
    {
        printf("Server not active\n");
        exit(1);
    }

    if ((msqid2 = msgget(key2, 0644)) == -1)
    {
        printf("Server not active\n");
        exit(1);
    }

    printf("Client 1 Ready :\n");
    requestMsg.type = 1;
    while (1)
    {
        fgets(requestMsg.text, sizeof(requestMsg.text), stdin);
        len = strlen(requestMsg.text);

        if (requestMsg.text[len - 1] == '\n')
        {
            requestMsg.text[len - 1] = '\0';
        }

        msgsnd(msqid1, &requestMsg, len + 1, 0);
        sleep(1);

        /*receiving respondMsg*/
        msgrcv(msqid2, &respondMsg, sizeof(respondMsg), 1, 0);

        printf("From server: %s\n", respondMsg.text);
    }
    printf("Server Disconnected\n");
}
