#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
 main ( )
{
   struct hostent *phe;
   char hostname [ 80 ];
   int  stat;
   unsigned long hostip;
   unsigned long hostiph;
   hostip=0;
   /* gethostname will place the name of the local host into hostname */
   stat = gethostname (hostname,80);
   printf ( "hostname is %s;  \n ", hostname ) ;
    /* gethostbyname will get the TCP/IP address of a named computer  
   and place it in phe->h_addr */
   if ((phe=gethostbyname(hostname)) != 0)
   {bcopy(phe->h_addr,&hostip,4);
    /* bcopy(from,toaddr,n) will copy n bytes from from to toaddr)*/
    /* ntohl will convert the byte order of a long integer from network order to      host order */
   hostiph = ntohl(hostip);
   printf ("Host IP address stored in hostip variable is %lx\n ",hostip );
   printf ("Host IP address stored in hostiph variable  %lx\n ",hostiph );
     printf ("\n");
   }
   else 
   {fprintf(stderr, "can't get \"%s\" host entry\n",hostname);
   exit(1);
   }
   exit(0);
   }
