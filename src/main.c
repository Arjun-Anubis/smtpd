#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <magic.h>

#include "log.h"
#include "network.h"

#define LOGLEVEL LOG_DEBUG

void smtp_handler( int fd, char * transmission, struct sockaddr_in address, struct RootConfig config )
{
	/* int requested_file; */
	/* char * headers[] = { "HTTP/1.1 200 OK", "Sever: Custom", "Content-Type: text/html", ""}; */
	/* char * headers[] = { "HTTP/1.1 200 OK", "Sever: Custom", "" }; */ 
	char * transmission_cpy;

	log_info( "In smtp handler" );

	transmission_cpy = malloc( strlen( transmission ) + 1 );
	strcpy( transmission_cpy, transmission );

	log_info( "Received header:\n%s", transmission );
	
}


int main( int argc, char ** argv ) 
{
	/* Options for our program, -l for log file,
	 * -f config file -p port -a bind address */
	/* -v for logging */

	FILE  * config_file;
	char config[1024];
	/* Change this later */
	struct json_object * json_config;
	struct json_object * address_json;
	struct json_object * port_json;
	struct RootConfig root_config;
	int opt;
	int port = 25;
	char address[ INET_ADDRSTRLEN ]  = "0.0.0.0";
	int loglevel = LOGLEVEL;

	log_set_level( loglevel );

	config_file = fopen( "config.json", "r" );
	fread( config, 1024, 1, config_file );
	json_config = json_tokener_parse( config );

	json_object_object_get_ex( json_config, "address", &address_json );
	json_object_object_get_ex( json_config, "port", &port_json );

	strncpy( address, json_object_get_string( address_json ), INET_ADDRSTRLEN );
	port = json_object_get_int( port_json );

	while ( ( opt = getopt( argc, argv, ":l:r:p:a:v" ) ) != -1 )
	{
		switch( opt )
		{
			case 'p':
				port = atoi( optarg );
				break;
			case 'a':
				strncpy( address, optarg, INET_ADDRSTRLEN );
				break;
			case 'v':
				log_set_level( --loglevel );
				break;
			case 'l':
				break;
			case 'r':
				 root_config.root_dir = create_astring( "Hello" );
				break;
			default:
				log_warn( "Unrecognized option %s", optarg );

		}
	}
	
	log_trace( "Set port: %d", port );
	log_trace( "Set address: %s", address );
	log_trace( "Set loglevel: %d", loglevel );


	strcpy( address, root_config.address_as_string );
	root_config.port = port;
	create_listener( root_config, smtp_handler );

	return 0;
}
