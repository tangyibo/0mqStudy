C/C++使用ZeroMQ的Router/Dealer模式搭建高性能后台服务框架
========================================

常见的后台服务模型
---------------------

      常见的服务框架：一个IO线程负责监听和接受客户端的请求，接收完请求后，封装成一个任务（Task），然后丢入任务队列管理器（TaskManager）中，至于怎么调度和管理这些任务（Task），任务管理器（TaskManager）就得采用一定的调度算法来管理和分派这些任务到线程池（ThreadPool）中，然后由指定的工作线程（Worker）来完成这些任务。同时工作线程（Worker）和任务管理器（TaskManager）还必须有一个通知机制，当Worker处理完Task的时候，能够高效的将结果给会到TaskManager，从而回复给请求的客户端。


基于ZeroMQ的服务模型
---------------------------

    简单使用ZeroMQ提供的ROUTER/DEALER组合模式，可以轻松搭建一个高性能异步的C/C++后台服务框架。

    ROUTER其实指的是ZeroMQ里面的一种套接字类型（ ZMQ_ROUTER类型）。这个套接字把接收到的请求（ZMQ_REQ）公平的排队分发，而且ROUTER还会标志客户的身份，从而确保能够将应答数据给到客户端。这样就刚好可以解决了我们想要的“监听和接受客户端请求，并丢入任务管理器”。【这里的任务管理器指的是DEALER】

    DEALER其实指的是ZeroMQ里面的一种套接字类型（ZMQ_DEALER）。这个套接字会把“任务”负载均衡的分发到后端的工作线程（或进程）Worker的。并且，当DEALER接受到Worker的处理结果的时候，DEALER还会把处理结果数据传递给ROUTER，由ROUTER将应答回给客户端。这样一来，DEALER也解决了我们想要的“任务管理调度和Worker分配工作的问题”。
   
    ROUTER可以高效的接收客户端的请求，而DEALER可以负载均衡的调度后端Worker工作。当客户端的请求特别多，后端Worker处理不过来，需要增加Worker的时候，也非常简单，新加入的Worker直接Connect到DEALER即可。如此运维起来也非常高效，后端可以非常简单的横向扩展！
    备注：值的一提的是，ROUTER又叫做XREP，DEALER又叫做XREQ。



