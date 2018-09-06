#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
 
int main (void)
{
    printf ("Connecting to hello world server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");
 
    for (int i = 0; i != 10; i++) 
    {
        char buffer [128];
        sprintf(buffer,"Hello-%d",i);
                
        printf ("Send: %s\n", buffer);
        zmq_send (requester, buffer, strlen(buffer)+1, 0);
        
        memset(buffer,0,sizeof(buffer));
        zmq_recv (requester, buffer, sizeof(buffer), 0);
        printf ("Recv:%s\n", buffer);
    }
    
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}