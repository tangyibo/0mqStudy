#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
 
int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);
 
    while (1) 
    {
        char buffer [128];
        
        zmq_recv (responder, buffer, sizeof(buffer), 0);
        printf ("recv: %s\n",buffer);
        
        sleep (1);          //  Do some 'work'
        
        zmq_send (responder, buffer,strlen(buffer)+1, 0);
    }
    return 0;
}