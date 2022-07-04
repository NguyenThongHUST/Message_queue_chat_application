
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
struct message
{
    long type;
    char text[200];
};

struct message requestMsg;

struct message respondClient1;
struct message respondClient2;
void main()
{
    int len, msqid1, msqid2;
    key_t key1 = 2102;
    key_t key2 = 3102;

    /* message queue for requestMsg */
    if ((msqid1 = msgget(key1, 0644 | IPC_CREAT)) == -1)
    {
        printf("Error creating message key1 message queue!\n");
        exit(EXIT_FAILURE);
    }

    /* message queue for respondMsg*/
    if ((msqid2 = msgget(key2, 0644 | IPC_CREAT)) == -1)
    {
        printf("Error creating message key2 message queue!\n");
        exit(EXIT_FAILURE);
    }

    printf("Server ready :\n");

    while (1)
    {
        /*receing requestMsg*/
        // if (msgrcv(msqid1, &requestMsg, sizeof(requestMsg.text), 1, 0) != -1)
        // {
        //     printf("From Client 1: \"%s\"\n", requestMsg.text);

        //     respondClient1.type = 1;
        //     char str1[6] = "ACK_1";
        //     strcpy(respondClient1.text, str1);

        //     /*Sending respondMsg*/
        //     msgsnd(msqid2, &respondClient1, strlen(respondClient1.text) + 1, 0);
        // }

        // if (msgrcv(msqid1, &requestMsg, sizeof(requestMsg.text), 2, 0) != -1)
        // {
        //     printf("From Client 2: \"%s\"\n", requestMsg.text);

        //     respondClient2.type = 2;
        //     char str2[6] = "ACK_2";
        //     strcpy(respondClient2.text, str2);

        //     /*sending respondMsg*/
        //     msgsnd(msqid2, &respondClient2, strlen(respondClient2.text) + 1, 0);
        // }

        if (msgrcv(msqid1, &requestMsg, sizeof(requestMsg.text), 0, 0) != -1)
        {
            if (requestMsg.type == 1)
            {
                printf("From Client 1: \"%s\"\n", requestMsg.text);
                respondClient1.type = 1;
                char str1[6] = "ACK_1";
                strcpy(respondClient1.text, str1);

                /*sending respondMsg*/
                msgsnd(msqid2, &respondClient1, strlen(respondClient1.text) + 1, 0);
            }

            else if (requestMsg.type == 2)
            {
                printf("From Client 2: \"%s\"\n", requestMsg.text);
                respondClient2.type = 2;
                char str2[6] = "ACK_2";
                strcpy(respondClient2.text, str2);

                /*sending respondMsg*/
                msgsnd(msqid2, &respondClient2, strlen(respondClient2.text) + 1, 0);
            }
        }
    }

    if (msgctl(msqid1, IPC_RMID, NULL) == -1)
    {
        printf("Error msgctl!\n");
        exit(EXIT_FAILURE);
    }
    if (msgctl(msqid2, IPC_RMID, NULL) == -1)
    {
        printf("Error msgctl!\n");
        exit(EXIT_FAILURE);
    }
}