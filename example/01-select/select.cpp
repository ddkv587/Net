#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DIE( X ) do { perror( X ); exit(-1); } while(0)

const static unsigned int s_uiServerPort = 8000;

int main(void)
{
    int nfds        = -1;

    int fdServer    = socket( AF_INET, SOCK_STREAM, 0 );

    if ( -1 == fdServer )   DIE( "listener" );

    sockaddr_in addrServer;
    addrServer.sin_family       = AF_INET;
    addrServer.sin_addr.s_addr  = htonl( INADDR_ANY );
    addrServer.sin_port         = htons( s_uiServerPort );

    socklen_t addrLen = sizeof( struct sockaddr );
    if ( -1 == bind( fdServer, ( struct sockaddr* )&addrServer, addrLen ) )  DIE( "bind" );

    fd_set  rfds;
    FD_ZERO( &rfds );
    FD_SET( fdServer, &rfds );

    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    char recvBuffer[ 1024 ];

    while ( 1 )
    {
        int retval = select( nfds + 1, &rfds, NULL, NULL, &tv );

        if ( -1 == retval ) {
            // error
        } else if ( 0 == retval ) {
            // time out
            printf( " select timeout\n " );
        } else {
            if ( FD_ISSET( fdServer, &rfds ) ) {
                // receive client
                struct sockaddr_in addrClient;
                int fdClient = accept( fdServer, ( struct sockaddr* )&addrClient, &addrLen );

                if ( fdClient ) DIE( "accept" );

                printf( " accept: %d, %d:%d\n ", fdClient, ntohl( addrClient.sin_addr.s_addr ), ntohs( addrClient.sin_port ) );

                FD_ISSET( fdClient, &rfds );
            }
            else {
                if ( read( fdClient ) ) {

                }
            }
        }
    }
    return 0;
}