#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler( int sig )
{
  void *array[10];
  size_t size;
  // get void *'s for al entries on the stack
  size = backtrace( array, 10 );

  // print out all the frames to stderr
  fprintf( stderr, "Error: signal %d", sig ) ;
  backtrace_symbols_fd( array, size, STDERR_FILENO );
  exit( 1);
}

// ------------------------ eof --------------------
