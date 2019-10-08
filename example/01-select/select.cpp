#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIE( X ) do { perror( X ); exit(-1); } while(0)

const static unsigned int s_uiServerPort = 8000;

int main( int argc, char const *argv[] )
{
    int nfds = -1;
    int fdServer;
    int fdClientArray[ 1024 ];
    int fdClientSize = 0;
    
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

    char recvBuffer[ 1024 ];
    memset( recvBuffer, 0, 1024 );

    while ( 1 ) {
        fd_set rfds;
        FD_ZERO( &rfds );
        FD_SET( fdServer, &rfds );
        nfds = fdServer;

        for ( int index = 0; index < fdClientSize; ++index ) {
            if ( -1 != fdClientArray[index] ) {
                FD_SET( fdClientArray[index], &rfds );
                nfds = nfds > fdClientArray[index] ? nfds : fdClientArray[index];
            }
        }

        timeval tv;
        tv.tv_sec   = 5;
        tv.tv_usec  = 0;
        int retval = select( nfds + 1, &rfds, NULL, NULL, &tv );

        if ( -1 == retval ) {
            // error
            printf( "select error\n" );
        } else if ( 0 == retval ) {
            // time out
            printf( "select timeout\n" );
        } else {
            if ( FD_ISSET( fdServer, &rfds ) ) {
                // receive client
                struct sockaddr_in addrClient;
                int fdClient = accept( fdServer, ( struct sockaddr* )&addrClient, &addrLen );

                if ( -1 == fdClient ) DIE( "accept" );

                printf( "accept: %d, %d:%d\n", fdClient, ntohl( addrClient.sin_addr.s_addr ), ntohs( addrClient.sin_port ) );

                FD_SET( fdClient, &rfds );
                nfds = nfds > fdClient ? nfds : fdClient;

                fdClientArray[ fdClientSize++ ] = fdClient;
            } else {
                for ( int index = 0; index < fdClientSize; ++index ) {
                    if ( FD_ISSET( fdClientArray[index], &rfds ) ) {
                        size_t rcvSize;
                        if ( !( rcvSize = read( fdClientArray[index], recvBuffer, 1024 ) ) ) {
                            printf( "%d closed! \n", fdClientArray[index] );
                            close( fdClientArray[index] );
                            FD_CLR( fdClientArray[index], &rfds );
                            fdClientArray[ index ] = -1;
                        } else {
                            printf( "recv data from %d: %s\n", fdClientArray[index], recvBuffer );
                            write( fdClientArray[index], recvBuffer, rcvSize );
                            memset( recvBuffer, 0, 1024 );
                        }
                    }
                }
            }
        }
    }
    return 0;
}