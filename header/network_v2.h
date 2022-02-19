#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#define CONNECTION_QUEUE_SIZE 5

struct DirConfig
{
	std::string path;
};


struct Config 
{
	Config( const char address[ INET_ADDRSTRLEN ], int port );
	char address[ INET_ADDRSTRLEN ];
	struct sockaddr_in socket_config;
	int port;
	struct DirConfig dirconfig;
};

struct listener
{
	int fd;
	void (*callback) ( int, char *, struct sockaddr_in );
	struct Config config;
	listener( void callback( int sockfd, char * transmission, struct sockaddr_in incoming_address ), struct Config config );
	~listener() {};
	int begin();
};
