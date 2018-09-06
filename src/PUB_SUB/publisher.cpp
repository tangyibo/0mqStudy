#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv)
{
    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    printf("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
    printf("===========================================\n\n");

    char addr[128] = {0};
    snprintf(addr, sizeof (addr), "tcp://*:6668");
    void* context = zmq_ctx_new();
    void* broadcast = zmq_socket(context, ZMQ_PUB);

    int rec = zmq_bind(broadcast, addr);
    if (rec)
    {
        printf("zmq_bind %s error:%d\n", addr, rec);
        zmq_close(broadcast);
        zmq_ctx_destroy(context);
        return -2;
    }

    while (1)
    {
        /* 要“广播”发布新配置的频道名称 */
        printf("please input channel:");
        char channel[20] = {0};
        //snprintf(channel, sizeof(channel), "ADMIN");
        scanf("%s", channel);

        if (strcmp(channel, "exit") == 0) break;

        printf("please input one broadcast msg:");

        char msg[1024] = {0};
        scanf("%s", msg);

        rec = zmq_send(broadcast, channel, strlen(channel), ZMQ_SNDMORE);
        printf("send context(%s) ... %d\n", channel, rec);

        rec = zmq_send(broadcast, msg, strlen(msg), 0);
        printf("send context(%s) ... %d\n", msg, rec);
    }

    zmq_close(broadcast);
    zmq_ctx_destroy(context);

    printf("good bye and good luck!\n");

    return 0;
}