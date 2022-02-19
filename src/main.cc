#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>

#include "network_v2.h"

void default_callback( int sockfd, char * transmission, struct sockaddr_in incoming_address )
{
	printf( "Doing some SMTP handling" );
}

int main( int argc, char ** argv )
{
	struct Config config( "192.168.1.37", 8080 );
	listener a( default_callback, config );
	printf( "Main: start listener" );
	a.begin();
}
