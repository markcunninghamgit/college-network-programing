/* udpechoc.c - main, udpecho */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
extern int errno;

#define LINELEN		128
struct sockaddr_in * getserveraddr ( char *, char * ) ;/* function prototype to return server address */


/*------------------------------------------------------------------------
 * main - UDP client for ECHO service
 *------------------------------------------------------------------------
 */
int main( argc, argv )
int argc;
char *argv[];
{
  char *host; 		 /*used to hold name of  host 			 */
  char *service;        /* used to hold port no. as a string  		*/

   if( argc == 3 ) {
   	host    = argv[ 1 ]; 
  	service = argv[ 2 ];
   		    }
  else {
       fprintf( stderr, "usage: UDPecho [ host [ port ] ]\n" );
       exit( 1 );
        }
  UDPecho( host, service );
  exit( 0 );
}

/*------------------------------------------------------------------------
 * UDPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int UDPecho( host, service )
char *host;
char *service;
{
char buff[120] ="test string",buffin[120];
int s;
int sockno ;				/*server socket number */
 struct sockaddr_in *addrptr;
int alen;
 int len1, len2;
  /* create socket for TCP/IP family , datagram service */
  /* put socket call in here */
  /* get the address of the server */
  /* put call to getservaddr in here */
  /* send a data gram */

  /* put sendto call in here */
 /* receive a datagram */

 /* put recvfrom call in here */
printf("string sent to echo server is %s \n",buff);
printf("string returned from echo server is %s\n",buffin);
return 0;
} 

/*------------------------------------------------------------------------
 * getsererveraddr - get server address and put it in servadd 
 *------------------------------------------------------------------------
 */
struct sockaddr_in *  getserveraddr(
   char *host, 
   char *service)	/* port number as a character string */
   
{
  static struct sockaddr_in servadd;   /* structure to hold server address */
  struct hostent *phe;
  int s, type;			/* socket descriptor and socket type	*/

/* use bzero function to zero out the structure servadd */

   bzero((char *)&servadd, sizeof( servadd ) ); 
/* insert the appropriate data in servadd for use as a TCP/IP datagram socket*/

	  servadd.sin_family = AF_INET; /* TCP/IP address family*/ 

  /* Convert service to integer and put it in servadd structure */
  if ( ( servadd.sin_port = htons( (ushort)atoi( service ) ) ) == 0 )
  {
    fprintf( stderr, "cant get \"%s\" service entry\n", service );
    exit(0  );
  }

  /* Map host name to IP address, and put it in servadd structure */

 if(phe = gethostbyname(host))
  bcopy (phe->h_addr,(char*)&servadd.sin_addr,phe->h_length);
   else
{fprintf(stderr, "can't get \"%s\"host entry\n",host);
   exit (0 );
} 
return (  &servadd); 
}
