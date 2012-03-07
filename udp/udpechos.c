/* udpechos.c - main */

#include <netdb.h> 
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
extern int errno;
int passiveudp(char*);
#define BUFLEN 120
/*------------------------------------------------------------------------
 * main - Iterative UDP server for ECHO service
 *------------------------------------------------------------------------
 */
int main( argc, argv )
int argc;
char *argv[];
{
  struct sockaddr_in fsin;		/* the from address of a client */
  char *service = " echo";		/* service name or port number	*/
  char buf[ BUFLEN ];			/* "input" buffer; any size > 0 */
  int sock;				/* server socket		*/
  int alen;				/* from-address length		*/

  switch( argc ) {
  case 1:
    break;
  case 2:
    service = argv[ 1 ];
    break;
  default:
    fprintf( stderr, "usage: udpechos [ port ]\n" );
  }

  sock = passiveudp( service );
  while( 1 ) {
    alen = sizeof( fsin );
    if ( recvfrom( sock, buf, sizeof( buf ), 0,
                   (struct sockaddr *)&fsin, &alen ) < 0 )
    {
      fprintf( stderr, "recvfrom: %s\n", strerror( errno ) );
      exit( 1 );
    }
    (void) sendto( sock, (char *)&buf, sizeof( buf), 0,
                   (struct sockaddr *)&fsin, sizeof( fsin ) );
  }
}
/*------------------------------------------------------------------------
 * passiveudp - allocate & bind a server socket using UDP
 *------------------------------------------------------------------------
 */
int passiveudp( 
   char *service)	/* service associated with the desired port	*/
{
  struct sockaddr_in sin;	/* an Internet endpoint address		*/
  int s;			/* socket descriptor            	*/

  bzero( (char *)&sin, sizeof( sin ) ); /* initialise address sin */
  sin.sin_family = AF_INET; /* put IP address family in member sin_family */ 
  sin.sin_addr.s_addr = INADDR_ANY;  /* put wildcard in for local IP address */

  /* convert port no. in 'service' from string to integer  */
  /* convert integer from host to network byte order */
  /* put resultant port no. into member sin.port of address sin */
  if ( ( sin.sin_port = htons( (ushort)atoi( service ) ) ) == 0 )
  {
    fprintf( stderr, "can't get \"%s\" service entry\n", service );
    exit( 1 );
  }
 /* local address is now in sin */
  /* Allocate a socket */
  s = socket( PF_INET, SOCK_DGRAM,0 );
  if ( s < 0 )
  {
    fprintf( stderr, "can't create socket: %s\n", strerror( errno ) );
    exit( 1 );
  }

  /* Bind the socket */
  if ( bind( s, (struct sockaddr *)&sin, sizeof( sin ) ) < 0 )
  {
    fprintf( stderr, "can't bind to %s port: %s\n", service, 
	     strerror( errno ) );
    exit( 1 );
  }

  return s;
}
