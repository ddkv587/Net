#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DIE( X ) do { perror( X ); exit(-1); } while(0)

const static unsigned int s_uiServerPort = 8000;

#define DEF_MAX_EVENTS  1024
#define DEF_TIMEOUT_MS  5000

void setNonBlock( int fd, bool on )
{
    int flags = fcntl( fd, F_GETFL, 0 );
    int ret = on ? fcntl( fd, F_SETFL, flags | O_NONBLOCK ) : fcntl( fd, F_SETFL, flags & ~O_NONBLOCK );
    
    if ( -1 == ret )    DIE( "fcntl" );
}

int main(int argc, char const *argv[])
{
    int fdServer;
    
    fdServer = socket( AF_INET, SOCK_STREAM, 0 );
    if ( -1 == fdServer )   DIE( "listener" );

    sockaddr_in addrServer;
    addrServer.sin_family       = AF_INET;
    addrServer.sin_addr.s_addr  = htonl( INADDR_ANY );
    addrServer.sin_port         = htons( s_uiServerPort );

    int on = 1;
    setsockopt( fdServer, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );

    socklen_t addrLen = sizeof( struct sockaddr );
    if ( -1 == bind( fdServer, ( struct sockaddr* )&addrServer, addrLen ) )  DIE( "bind" );
    if ( -1 == listen( fdServer, 1024 ) )  DIE( "listen" );
    setNonBlock( fdServer, true );

    char recvBuffer[ 1024 ];
    memset( recvBuffer, 0, 1024 );

    int epollfd = epoll_create( DEF_MAX_EVENTS );
    if ( -1 == epollfd )  DIE( "epoll_create" );

    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fdServer;
    if ( -1 == epoll_ctl( epollfd, EPOLL_CTL_ADD, fdServer, &event ) )  DIE( "epoll_ctl" );

    epoll_event eventArray[ DEF_MAX_EVENTS ];

    while ( 1 ) {
        int nfds = epoll_wait( epollfd, eventArray, DEF_MAX_EVENTS, DEF_TIMEOUT_MS );
        if ( -1 == nfds )   DIE( "epoll_wait" );

        if ( 0 == nfds ) {
            printf( "epoll_wait timeout\n" );
        } else {
            for ( int index = 0; index < nfds; ++index ) {
                if( ( eventArray[index].events & EPOLLERR )    ||
                        ( eventArray[index].events & EPOLLHUP )   ||
                        ( !( eventArray[index].events & EPOLLIN) ) ) {
                    printf( "epoll error\n" );
                    close( eventArray[index].data.fd );
                    continue;
                }                

                if ( eventArray[index].data.fd == fdServer ) {
                    while ( 1 ) {
                        // accept new client
                        struct sockaddr_in addrClient;
                        int fdClient = accept( fdServer, ( struct sockaddr* )&addrClient, &addrLen );

                        if ( -1 == fdClient ) {
                            if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
                                printf( "accept all client\n" );
                                break;
                            } else {
                                DIE( "accept" );
                            }
                        }

                        printf( "accept: %d, %d:%d\n", fdClient, ntohl( addrClient.sin_addr.s_addr ), ntohs( addrClient.sin_port ) );
                        setNonBlock( fdClient, true );
                        event.events    = EPOLLIN | EPOLLET;
                        event.data.fd   = fdClient;
                        if ( -1 == epoll_ctl( epollfd, EPOLL_CTL_ADD, fdClient, &event ) )  DIE( "epoll_ctl" );
                    }
                } else {
                    // receive data
                    int fdClient = eventArray[index].data.fd;

                    int rcvSize = 0;
                    int size = 0;
                    while ( ( size = read( fdClient, recvBuffer + rcvSize, 1024 - rcvSize ) ) > 0 ) {
                        rcvSize += size;
                    }
                    if ( size == 0 ) {
                        // close client
                        printf( "%d closed! \n", fdClient );
                        //if ( -1 == epoll_ctl( epollfd, EPOLL_CTL_DEL, fdClient, NULL ) )  DIE( "epoll_ctl" );
                        close( fdClient );
                    } else {
                        if ( errno == EAGAIN || errno == EWOULDBLOCK ) {
                            printf( "recv data from %d: %s\n", fdClient, recvBuffer );
                            write( fdClient, recvBuffer, rcvSize );
                            memset( recvBuffer, 0, 1024 );
                        } else {
                            // error
                            DIE( "read" );
                        }
                    }
                }
            }
        }
    }
    return 0;
}
