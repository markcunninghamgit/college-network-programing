/* server-tcp.c -  */
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define QLEN		   5	/* maximum connection queue length	*/
#define BUFSIZE		4096
extern int	errno;

/*------------------------------------------------------------------------
 * main - Iterative TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int main( int argc, char *argv[] )
{
	char *service;			/* service   port number	*/
	struct sockaddr_in clientAddress;		/* the address of a client	*/
	int clientAddressLength;				/* length of client's address	*/
	int listenerSocket;				/* master server socket		*/
	int clientConnectionSocket;				/* slave server socket		*/

	if (argc != 2 )	
	{
		fprintf( stderr, "usage: tcpechos  port \n" );
		exit( 1 );
	}

	service = argv[1];
	/* call passivetcp to create socket, and put it in passive listening mode */
	listenerSocket = passivetcp( service, QLEN );

	while ( 1 )
	{
		clientAddressLength = sizeof( clientAddress );
		clientConnectionSocket = accept( listenerSocket, ( struct sockaddr * ) &clientAddress, &clientAddressLength );

		if ( clientConnectionSocket < 0 )
		{
			fprintf( stderr, "accept: %s\n", strerror( errno ) );
			exit( 1 );
		}

		TCPechod( clientConnectionSocket ) ;
		( void ) close( clientConnectionSocket );
	}
}
/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int TCPechod( fileDescriptor )
	int fileDescriptor;
{
	char inputBuffer[ BUFSIZE ];
	int charactersRead;

	do
	{
		/* read characters from the socket; charactersRead = no. of chars read */
		/* charactersRead =0 if end-of-file  (^d) is read */
		/* charactersRead = negative if error occurs in read */
		charactersRead = read( fileDescriptor, inputBuffer, sizeof( inputBuffer ) ); 

		if ( charactersRead < 0 )
		{
			fprintf( stderr, "echo read: %s\n", strerror(errno ) );
			exit( 1 );
		}

		/* write the buffer contents back to the socket */
		if ( write( fileDescriptor, inputBuffer, charactersRead ) < 0 )

		{
			fprintf( stderr, "echo write: %s\n", strerror( errno ) );
			exit( 1 );
		}
	} while (charactersRead > 0 ); /* read until charactersRead = 0 */

	return 0;
}
/*------------------------------------------------------------------------
 * passivetcp - allocate & bind a server socket using TCP
 *------------------------------------------------------------------------
 */
int passivetcp( 
		char *service,	/* port no. of the desired port	*/
		int qlen )		/* maximum length of the server request queue	*/
{
	struct sockaddr_in clientAddress;	/* an Internet endpoint address		*/
	int listenSocket ;			/* socket descriptor */
	int retcode;			/* return code from function call */

	/* initialise clientAddress with  0's */
	bzero( (char *)&clientAddress, sizeof( clientAddress ) );

	/* put address family  AF_INET into sin.sin_family*/
	clientAddress.sin_family = AF_INET;

	/* put in INADDR_ANY into IP address element of sin so socket software can use     any local IP address */
	clientAddress.sin_addr.s_addr = INADDR_ANY;

	/* convert  service (port no.) to integer */
	/* atoi converts  ascii string service to integer */
	/* htons converts short integer from host to network byte order*/
	/* put port number in sin.sin_port to complete the address in sin */
	retcode = clientAddress.sin_port = htons( (ushort)atoi( service ) ) ;

	if (retcode == 0)
	{
		fprintf( stderr, "can't get \"%s\" service entry\n", service );
		exit( 1 );
	}

	/* Allocate a socket */
	listenSocket = socket( PF_INET, SOCK_STREAM, 0);

	if ( listenSocket < 0 )
	{
		fprintf( stderr, "can't create socket: %s\n", strerror( errno ) );
		exit( 1 );
	}

	/* Bind the socket  i.e fill in the local address into it*/
	if ( bind( listenSocket, (struct sockaddr *)&clientAddress, sizeof( clientAddress ) ) < 0 )
	{
		fprintf( stderr, "can't bind to %s port: %s\n", service, strerror( errno ) );
		exit( 1 );
	}

	/* put the socket in passive listening mode */
	/*  allow up to 5 requests to be  queued  */ 
	if  (listen( listenSocket, qlen ) < 0 )
	{
		fprintf( stderr, "can't listen on %s port: %s\n", service, strerror( errno ) );
		exit( 1 );
	}

	return listenSocket;
}
