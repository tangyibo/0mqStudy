#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int readOneMsg(void* socket)
{
    int64_t rcvmore = 0;
    size_t sz = sizeof (rcvmore);
    char configure[128] = {0};
    
    do
    {
        /* Create an empty OMQ message to hold the message part */
        zmq_msg_t part;
        int rc = zmq_msg_init(&part);
        if (rc)
        {
            printf("readOneMsg zmq_msg_init error:%d\n", rc);
        }
        
        /* Block until a message is available to be received from socket */
        rc = zmq_recvmsg(socket, &part, 0);
        if (rc < 0)
        {
            printf("readOneMsg zmq_recvmsg error:%d\n", rc);
        }
        else
        {
            printf("zmq_recvmsg (%d)=%.*s\n", rc, rc, (char*) zmq_msg_data(&part));
            snprintf(configure, sizeof (configure), "%.*s", rc, (char*) zmq_msg_data(&part));
        }
        
        rc = zmq_getsockopt(socket, ZMQ_RCVMORE, &rcvmore, &sz);
        zmq_msg_close(&part);
        
    } while (rcvmore);
    
    printf("recv new value is :\n%s\n", configure);
    
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("usage : %s topic\n", argv[0]);
        return -1;
    }

    int major, minor, patch;
    zmq_version(&major, &minor, &patch);
    printf("Current 0MQ version is %d.%d.%d\n", major, minor, patch);
    printf("===========================================\n\n");

    char addr[128] = {0};
    snprintf(addr, sizeof (addr), "tcp://localhost:6668");
    void* context = zmq_ctx_new();
    void* reader = zmq_socket(context, ZMQ_SUB);

    printf("subscribe topic is %s\n", argv[1]);
    int rec = zmq_setsockopt(reader, ZMQ_SUBSCRIBE, argv[1], strlen(argv[1]));
    if (rec)
    {
        printf("zmq_setsockopt error:%d\n", rec);
        zmq_close(reader);
        zmq_ctx_destroy(context);
        return -2;
    }

    rec = zmq_connect(reader, addr);
    if (rec)
    {
        printf("zmq_connect %s error:%d\n", addr, rec);
        zmq_close(reader);
        zmq_ctx_destroy(context);
        return -2;
    }

    zmq_pollitem_t items [] = {
        { reader, 0, ZMQ_POLLIN, 0}
    };

    while (1)
    {
        rec = zmq_poll(items, sizeof (items) / sizeof (zmq_pollitem_t), 10000);
        if (rec < 0)
        {
            printf("zmq_poll error:%d\n", rec);
            break;
        }
        if (items[0].revents & ZMQ_POLLIN)
        {
            printf("zmq_poll ZMQ_POLLIN event coming...\n");
            readOneMsg(reader);
        }
        else if (items[0].revents & ZMQ_POLLOUT)
        {
            printf("zmq_poll ZMQ_POLLOUT event coming...\n");
        }
        else if (items[0].revents & ZMQ_POLLERR)
        {
            printf("zmq_poll ZMQ_POLLERR\n");
        }
        else
        {
            printf("zmq_poll no event ready...\n");
        }
    }

    zmq_close(reader);
    zmq_ctx_destroy(context);

    printf("good bye and good luck!\n");

    return 0;
}