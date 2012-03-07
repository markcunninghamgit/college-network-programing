/* fork.c - a convential C program that sums integers from 1 to 5	*/

#include <stdio.h>

main()
{
   int sum ;
   int i ;

   sum = 0 ;
   fork() ;
   for ( i = 0 ; i != 10 ; i++ )
   {
      printf ( "the value of i is %d\n", i ) ;
      fflush ( stdout ) ;
      sum += i ;
	sleep (1);
   }
   printf ( "the sum is %d\n", sum ) ;
   return 0 ;
}
