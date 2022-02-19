#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_v2.h"

Config::Config( const char address[ INET_ADDRSTRLEN ], int port ) 
{
	memcpy( this->address, address, INET_ADDRSTRLEN ); // Could be +1

	/*
	 * Setting Socket config
	 */

	inet_aton( this->address, &(this->socket_config.sin_addr) ); // a->n
	this->socket_config.sin_port = htons( port );
	this->socket_config.sin_family = AF_INET;

	printf( "Set address to %s\n", this->address );
	printf( "Initialized config...\n" );
}

listener::listener( void callback( int sockfd, char * transmission, struct sockaddr_in incoming_address ), Config config )
   	/* :callback(callback), config(config) */
{
	printf( "Setting STMP Callback\n" );
	this->callback = callback;
	memcpy( this.config, config, sizeof( config ) );
};

int listener::begin()
{

	int opt = 1;
	/*
	 * Setting up socket
	 */

	printf( "Starting up listener..." );


	if ( ( this->fd = socket ( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
	{
		perror ( "socket error: " );
		return errno;
	}
	if ( setsockopt( this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt) ) )
	{
		perror ( "setsockopt: " );
		return errno;
	}
	if ( bind( this->fd, ( struct sockaddr * )&this->config.socket_config, sizeof( this->config.socket_config ) ) < 0 )
	{
		perror( "Bind failed" );
		return errno;
	}	
	if ( listen( this->fd, CONNECTION_QUEUE_SIZE ) < 0 )
	{
		perror( "listen" );
		return errno;
	}

	/*
	 * Accepting Connections
	 * Forever
	 */

	for ( int fork_pid, connection_number = 0; ; connection_number++ )
	{
		int connection;
		struct sockaddr_in incoming;
		const int address_length = sizeof( this->config.socket_config );

		printf( "Waiting for connections..." );
		if ( ( connection = accept( this->fd, ( struct sockaddr * )&incoming, ( socklen_t * )&address_length ) ) < 0 ) 
			/*
			 * This will halt till connection received
			 */
		{
			perror( "accept" );
			return errno;
		}

		/*
		 * Forking
		 */

		printf( "Connection from %s port %d connection number %d", inet_ntoa( incoming.sin_addr ), incoming.sin_port, connection_number );

		if ( ( fork_pid = fork() ) == 0 )
		{
			/* 
			 * Child Proccess
			 */
			
			/*
			 * Receive transmission and call callback
			 */

			printf( "Inside Fork" );
			/* int read_size = 12; */
			
			/* for ( int transmission_number = 0; read_size > 0; transmission_number++ ) */
			/* { */
			/* 	int transmission_size = 0; */
			/* 	char * transmission = malloc(1); */

			/* 	for */
			/* 	( */
			/* 	 int packet_number = 0; */
			/* 	 read_size = read( this->fd, */ 
			/* 	) */

			/* } */
		}
		else
		{
			/*
			 * Parent Process
			 */
			printf( "Assigned connection number %d, from %s to fork_pid %d", connection_number, inet_ntoa( incoming.sin_addr ), fork_pid );
			printf( "Return to listen" );
			continue;
		}

	}

};

