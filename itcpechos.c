/* itcpechos.c - main, */
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
  struct sockaddr_in fsin;		/* the address of a client	*/
  int alen;				/* length of client's address	*/
  int msock;				/* master server socket		*/
  int ssock;				/* slave server socket		*/
  if (argc != 2 )	
  {
      fprintf( stderr, "usage: tcpechos  port \n" );
      exit( 1 );
   }
  service = argv[1];
  /* call passivetcp to create socket, and put it in passive listening mode */
  msock = passivetcp( service, QLEN );
  while ( 1 )
  {
    alen = sizeof( fsin );
    ssock = accept( msock, ( struct sockaddr * ) &fsin, &alen );
    if ( ssock < 0 )
    {
      fprintf( stderr, "accept: %s\n", strerror( errno ) );
      exit( 1 );
    }
         TCPechod( ssock ) ;
        ( void ) close( ssock );
  }
}
/*------------------------------------------------------------------------
 * TCPechod - echo data until end of file
 *------------------------------------------------------------------------
 */
int TCPechod( fd )
int fd;
{
  char buf[ BUFSIZE ];
  int ccount;
  do
{
/* read characters from the socket; ccount = no. of chars read */
/* ccount =0 if end-of-file  (^d) is read */
/* ccount = negative if error occurs in read */
  ccount = read( fd, buf, sizeof( buf ) ); 
    if ( ccount < 0 )
    {
      fprintf( stderr, "echo read: %s\n", strerror(errno ) );
      exit( 1 );
    }
/* write the buffer contents back to the socket */
    if ( write( fd, buf, ccount ) < 0 )
	    
    {
      fprintf( stderr, "echo write: %s\n", strerror( errno ) );
      exit( 1 );
    }
}
/* read until ccount = 0 */
    while (ccount >0);
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
  struct sockaddr_in sin;	/* an Internet endpoint address		*/
  int s ;			/* socket descriptor */
  int retcode;			/* return code from function call */
  /* initialise sin with  0's */
  bzero( (char *)&sin, sizeof( sin ) );
  /* put address family  AF_INET into sin.sin_family*/
  sin.sin_family = AF_INET;
  /* put in INADDR_ANY into IP address element of sin so socket software can use     any local IP address */
  sin.sin_addr.s_addr = INADDR_ANY;

  /* convert  service (port no.) to integer */
  /* atoi converts  ascii string service to integer */
  /* htons converts short integer from host to network byte order*/
  /* put port number in sin.sin_port to complete the address in sin */
  retcode = sin.sin_port = htons( (ushort)atoi( service ) ) ;
  if (retcode == 0)
  {
    fprintf( stderr, "can't get \"%s\" service entry\n", service );
    exit( 1 );
  }

  /* Allocate a socket */
  s = socket( PF_INET, SOCK_STREAM, 0);
  if ( s < 0 )
  {
    fprintf( stderr, "can't create socket: %s\n", strerror( errno ) );
    exit( 1 );
  }
  /* Bind the socket  i.e fill in the local address into it*/
  if ( bind( s, (struct sockaddr *)&sin, sizeof( sin ) ) < 0 )
  {
    fprintf( stderr, "can't bind to %s port: %s\n", service, 
	     strerror( errno ) );
    exit( 1 );
  }
  /* put the socket in passive listening mode */
  /*  allow up to 5 requests to be  queued  */ 
  if  (listen( s, qlen ) < 0 )
  {
    fprintf( stderr, "can't listen on %s port: %s\n", service,
             strerror( errno ) );
    exit( 1 );
  }

  return s;
}
