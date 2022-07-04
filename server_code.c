

#include "svmsg_file.h"

void serveRequest(struct requestMsg *req)
{
    int fd;
    int numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1)
    {
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");

        if (msgsnd(req->clientId, &resp, strlen(resp.data) + 2, 0) == -1)
        {
            printf("Error sending message queue in RESP_MT_FAILURE!\n");
            return;
        }
        exit(EXIT_FAILURE);
    }

    resp.mtype = RESP_MT_DATA;
    while (numRead = read(fd, resp.data, REQ_MSG_SIZE))
    {
        if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
            break;
    }

    resp.mtype = RESP_MT_END;
    if (msgsnd(req->clientId, &resp, 0, 0) == -1)
    {
        printf("Error sending message queue in RESP_MT_END!\n");
        return;
    }
}

int main()
{
    struct requestMsg req;
    pid_t pid;
    int msgLen;
    int serverId;

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL);
    if (serverId == -1)
    {
        printf("Error creating message queue server! \n");
        return 1;
    }

    while (1)
    {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1)
        {
            // if (errno == EINTR)
            //     continue;
            printf("Error receiving client request!\n");
            // break;
        }
        pid = fork();
        if (pid == -1)
        {
            printf("Error fork()!\n");
            return 2;
        }

        if (pid == 0)
        {
            serveRequest(&req);
            exit(EXIT_SUCCESS);
        }
    }

    if (msgctl(serverId, IPC_RMID, NULL) == -1)
    {
        printf("Error msgctl!\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}