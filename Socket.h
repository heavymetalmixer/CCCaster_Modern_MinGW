#pragma once

#include "Thread.h"
#include "BlockingQueue.h"

#include <netlink/socket.h>
#include <netlink/socket_group.h>

#include <string>
#include <memory>
#include <unordered_map>

#define READ_BUFFER_SIZE ( 1024 * 4096 )

#define NL_SOCKET_GROUP_CMD(NAME)                                   \
    class NAME : public NL::SocketGroupCmd {                        \
        Socket& context;                                            \
    public:                                                         \
        NAME ( Socket& context ) : context ( context ) {}           \
        void exec ( NL::Socket *, NL::SocketGroup *, void * );      \
    }

class Socket
{
    class ListenThread : public Thread
    {
        Socket& context;
        volatile bool isListening;

    public:

        ListenThread ( Socket& context )
            : context ( context ), isListening ( false ) {}
        void start();
        void join();
        void run();
    };

    class ConnectThread : public Thread
    {
        Socket& context;
        const std::string addr;
        const int port;

    public:

        ConnectThread ( Socket& context, std::string addr, int port )
            : context ( context ), addr ( addr ), port ( port ) {}
        void start();
        void run();
    };

    char readBuffer[READ_BUFFER_SIZE];

    struct ReaperThread : public Thread { void run(); void join(); };

    std::shared_ptr<NL::Socket> serverSocket;
    std::shared_ptr<NL::Socket> tcpSocket, udpSocket;
    std::shared_ptr<NL::SocketGroup> socketGroup;
    std::unordered_map<uint32_t, std::shared_ptr<NL::Socket>> acceptedSockets;

    NL_SOCKET_GROUP_CMD ( Accept ) socketAcceptCmd;
    NL_SOCKET_GROUP_CMD ( Disconnect ) socketDisconnectCmd;
    NL_SOCKET_GROUP_CMD ( Read ) socketReadCmd;

    ListenThread listenThread;

    static BlockingQueue<std::shared_ptr<ConnectThread>> connectingThreads;

    static ReaperThread reaperThread;

    void addSocketToGroup ( const std::shared_ptr<NL::Socket>& socket );

protected:

    mutable Mutex mutex;

    virtual void accepted ( uint32_t id ) {}

    virtual void connected ( uint32_t id ) {}

    virtual void disconnected ( uint32_t id ) {}

    virtual void received ( char *bytes, std::size_t len, uint32_t id ) {}

    virtual void received ( char *bytes, std::size_t len, const std::string& addr, unsigned port ) {}

public:

    Socket();
    virtual ~Socket();

    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }

    void listen ( unsigned port );

    void connect ( std::string addr, unsigned port );

    void relay ( std::string room, std::string server, unsigned port );

    void disconnect ( uint32_t id = 0 );

    bool isServer() const;
    bool isConnected() const;

    std::string localAddr() const;
    std::string remoteAddr ( uint32_t id = 0 ) const;

    void send ( char *bytes, std::size_t len );
    void sendReliable ( char *bytes, std::size_t len, uint32_t id = 0 );

    static void release();
};