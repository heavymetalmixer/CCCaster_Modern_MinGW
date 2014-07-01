#include "Test.Socket.h"
#include "TcpSocket.h"

// TEST ( Socket, UdpSend )
// {
//     struct TestSocket : public Socket::Owner, public Timer::Owner
//     {
//         shared_ptr<Socket> socket;
//         Timer timer;
//         MsgPtr msg;
//         bool sent;
//
//         void readEvent ( Socket *socket, const MsgPtr& msg, const IpAddrPort& address ) override
//         {
//             this->msg = msg;
//
//             if ( socket->getRemoteAddress().addr.empty() )
//             {
//                 socket->send ( new TestMessage ( "Hello client!" ), address );
//                 sent = true;
//             }
//         }
//
//         void timerExpired ( Timer *timer ) override
//         {
//             if ( !sent )
//             {
//                 if ( !socket->getRemoteAddress().addr.empty() )
//                 {
//                     socket->send ( new TestMessage ( "Hello server!" ) );
//                     sent = true;
//                 }
//
//                 timer->start ( 1000 );
//             }
//             else
//             {
//                 EventManager::get().stop();
//             }
//         }
//
//         TestSocket ( unsigned port )
//             : socket ( Socket::listen ( this, port, Socket::Protocol::UDP ) )
//             , timer ( this ), sent ( false )
//         {
//             timer.start ( 1000 );
//         }
//
//         TestSocket ( const string& address, unsigned port )
//             : socket ( Socket::connect ( this, address, port, Socket::Protocol::UDP ) )
//             , timer ( this ), sent ( false )
//         {
//             timer.start ( 1000 );
//         }
//     };
//
//     TestSocket server ( TEST_LOCAL_PORT );
//     TestSocket client ( "127.0.0.1", TEST_LOCAL_PORT );
//
//     EventManager::get().start();
//
//     EXPECT_TRUE ( server.socket.get() );
//     if ( server.socket.get() )
//         EXPECT_TRUE ( server.socket->isServer() );
//
//     EXPECT_TRUE ( server.msg.get() );
//
//     if ( server.msg.get() )
//     {
//         EXPECT_EQ ( server.msg->getMsgType(), MsgType::TestMessage );
//         EXPECT_EQ ( server.msg->getAs<TestMessage>().str, "Hello server!" );
//     }
//
//     EXPECT_TRUE ( client.socket.get() );
//     if ( client.socket.get() )
//         EXPECT_TRUE ( client.socket->isConnected() );
//
//     EXPECT_TRUE ( client.msg.get() );
//
//     if ( client.msg.get() )
//     {
//         EXPECT_EQ ( client.msg->getMsgType(), MsgType::TestMessage );
//         EXPECT_EQ ( client.msg->getAs<TestMessage>().str, "Hello client!" );
//     }
// }

TEST_CONNECT                ( TcpSocket, 0, 0, 1000 )

TEST_TIMEOUT                ( TcpSocket, 0, 0, 1000 )

TEST_CLIENT_DISCONNECT      ( TcpSocket, 0, 0, 1000 )

TEST_SERVER_DISCONNECT      ( TcpSocket, 0, 0, 1000 )

TEST_SEND                   ( TcpSocket, 0, 0, 1000 )

TEST_SEND_WITHOUT_SERVER    ( TcpSocket, 0, 0, 1000 )

TEST_SEND_PARTIAL           ( TcpSocket )