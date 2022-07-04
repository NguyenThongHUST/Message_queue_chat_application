
#include "svmsg_file.h"

int clientId;

void removeQueue()
{
    if (msgctl(clientId, IPC_RMID, NULL) == -1)
    {
        exit(EXIT_FAILURE);
    }
}

int main()
{
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    /* Get server's queue identifier; create queue for response */
    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1)
    {
        return 1;
    }
    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1)
        return 2;

    /*client sending request*/
    req.mtype = 12;
    req.clientId = clientId;
    char pathname = "test.txt";

    strcpy(req.pathname, pathname);

    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
    {
        printf("Not sending succesfull!\n");
        return 2;
    }

    /*receive first notification*/
    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1)
        return 2;
    if (resp.mtype == RESP_MT_FAILURE)
    {
        printf("%s\n", resp.data); /* Display msg from server */
        if (msgctl(clientId, IPC_RMID, NULL) == -1)
            return 2;
        exit(EXIT_FAILURE);
    }

    totBytes = msgLen; /* Count first message */
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++)
    {
        msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if (msgLen == -1)
            return 2;
        totBytes += msgLen;
    }
    printf("Received %ld bytes (%d messages)\n", (long)totBytes, numMsgs);
    exit(EXIT_SUCCESS);
}