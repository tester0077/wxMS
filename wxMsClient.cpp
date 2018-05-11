/*
From: http://www.exegesis.uklinux.net/gandalf/winsock/winsock2.htm
*/
#if 0
/*
 * popclnt1.c
 *
 *
 * A very basic pop client that retrieves mail from an account into
 * a multi pop retrieval client in later parts of the tutorial with all
 * the extra error handling that's needed. Use this as a framework to
 * develop your own pop client. This program doesn't delete any mails
 * from the pop mail server, so you can try it out without being afraid
 * of losing any emails!
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winsock.h>

#define WIN32_LEAN_AND_MEAN

void handle_error(void);
void pop_error(void);
void write_file(int file, char *p);

int main (int argc, char *argv[])
{
  const long BUF_LEN=60000;

  /* 60000 char buffer. In real life we would have to allow for a much
   * larger buffer than this to allow retrieval of long messages which
   * might have attachments. We will be doing this in one of the later
   * parts of the tutorial by using synchronised data flow with a smaller
   * buffer that downloads mails in smaller chunks.
   */

  const char POP_ERR_MSG[]="-ERR";          /* POP Server error response */
  const int LOG_FILE=1;                     /* write to log file */
  const int MAIL_FILE=2;                    /* write to mail file */
  const int SCREEN=4;                       /* display on screen */
  WORD wVersionRequested;                   /* socket dll version info */
  WSADATA wsaData;                          /* socket lib initialisation */
  SOCKET sock;                              /* socket details */
  struct sockaddr_in address;               /* socket address stuff */
  struct hostent * host;                    /* host stuff */
  int err;                                  /* error trapping */
  float socklib_ver;                        /* socket dll version */
  char * File_Buf;                          /* file buffer */
  char * Rec_Buf;                           /* recieve buffer */

  /* Fill in to suit your own account, increase array sizes if needed */

  char hostname[20] = "pop.wherever.net";   /* hostname for pop server */
  char username[20] = "username";           /* username */
  char password[20] = "password";           /* password */

  int i,j;                                  /* general counters */
  char tempbuf[20] = "";                    /* temp string buffer for parsing */
  int num_flag=0;                           /* string parse flag */
  time_t now;                               /* from time.h */

  /* allocate dynamic storage from the heap */

  File_Buf = malloc(BUF_LEN * sizeof(char) + 1);
  Rec_Buf = malloc(BUF_LEN * sizeof(char) + 1);

  wVersionRequested = MAKEWORD( 1, 1 );

  if ( WSAStartup( wVersionRequested, &wsaData ) != 0 )
    handle_error();

  /* Check socket DLL supports 1.1 or higher */

  socklib_ver = HIBYTE( wsaData.wVersion ) / 10.0;
  socklib_ver += LOBYTE( wsaData.wVersion );

  if ( socklib_ver   {
    printf ("Error: socket library must support 1.1 or greater\n");
    WSACleanup();
    return 0;
  }

  /* get date and time to write to log file */

  time(&now);

  sprintf(File_Buf,"Pop Client Session started %.24s.\n\n", ctime(&now));
  write_file(LOG_FILE | SCREEN, File_Buf);

  /* open a socket based on TCP/IP */

  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET )
    handle_error();

  address.sin_family=AF_INET;               /* internet */
  address.sin_port = htons(110);            /* pop port */

  sprintf(File_Buf,"Connecting to %s\n", hostname);
  write_file(LOG_FILE | SCREEN, File_Buf);

  /* DNS on hostname */

  if ( (host=gethostbyname(hostname)) == NULL )
    handle_error();

  /* IP address for hostname */

  address.sin_addr.s_addr=*((unsigned long *) host->h_addr);

  /* try to connect to pop server */

  if ( (connect(sock,(struct sockaddr *) &address, sizeof(address))) != 0)
  handle_error();

  sprintf(File_Buf, "Connected to %s\n", hostname);
  write_file(LOG_FILE | SCREEN, File_Buf);

  err=recv(sock,Rec_Buf,BUF_LEN,0);             /* get response from server */
  sprintf(File_Buf,"received: %s", Rec_Buf);    /* should start +OK */
  write_file(LOG_FILE, File_Buf);

  if ( strstr(Rec_Buf,POP_ERR_MSG) != NULL )    /* check for errors */
    pop_error();

  strcpy(Rec_Buf,"USER ");
  strcat(Rec_Buf, username);
  strcat(Rec_Buf, "\r\n");
  err=send(sock,Rec_Buf,strlen(Rec_Buf),0);     /* send username */
  sprintf(File_Buf,"sent: %s",Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  err=recv(sock,Rec_Buf,BUF_LEN,0);             /* should recieve +OK */
  sprintf(File_Buf,"received: %s", Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  if ( strstr(Rec_Buf,POP_ERR_MSG) != NULL )    /* check for errors */
    pop_error();

  strcpy(Rec_Buf,"PASS ");
  strcat(Rec_Buf, password);
  strcat(Rec_Buf, "\r\n");
  err=send(sock,Rec_Buf,strlen(Rec_Buf),0);     /* send password */
  sprintf(File_Buf,"sent: %s",Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  err=recv(sock,Rec_Buf,BUF_LEN,0);             /* should recieve +OK */
  sprintf(File_Buf,"received: %s",Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  if ( strstr(Rec_Buf,POP_ERR_MSG) != NULL )    /* check for errors */
    pop_error();

  strcpy(Rec_Buf,"STAT\r\n");                  /* send stat command */
  err=send(sock,Rec_Buf,strlen(Rec_Buf),0);
  sprintf(File_Buf,"sent: %s", Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  /*
   * should recieve +OK x y where x = number of messages
   * and y is total octets. Note the buffers we have allocated
   * would not be sufficient for a real pop client. They
   * should be dynamically allocated here after checking the
   * size of the waiting emails.
   */

  err=recv(sock,Rec_Buf,BUF_LEN,0);
  sprintf(File_Buf,"received: %s", Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  if ( strstr(Rec_Buf,POP_ERR_MSG) != NULL )    /* check for errors */
    pop_error();

  /*
   * Now we need to work out how many emails we have to retrieve
   * and set up a loop to get them all.
   */

  i = j = 0;

  while (i   {
    if (Rec_Buf[i] > '0' && Rec_Buf[i]     {
      while (Rec_Buf[i] != ' ')                   /* space is end of number */
      {
        tempbuf[j] = Rec_Buf[i];
        j++;
        i++;
      }

      num_flag=1;
      tempbuf[j] = '\0';
    }

    i++;
  }

  i = atoi(tempbuf);                            /* number of emails */

  printf("There are %d message(s) waiting\r\n", i);

  /*
   * At this point we should really be checking the length of each email
   * on the POP server and then either allocate dynamic memory, or better
   * still, try synchronised access which we will be doing in a later
   * part of the tutorial. Until then, let's keep it simple. If you do
   * have an email larger than the buffer, it won't do anything else
   * other than crash the program!
   */

  for( j=1; j<=i; j++)
  {
    sprintf(Rec_Buf, "RETR %d\r\n", j);         /* add number */
    printf("Retrieving message number %d\n", j);
    err=send(sock,Rec_Buf,strlen(Rec_Buf),0);
    sprintf(File_Buf,"sent: %s ",Rec_Buf);
    write_file(LOG_FILE, File_Buf);

    err=recv(sock,Rec_Buf,BUF_LEN,0);           /* +OK message will follow */
    sprintf(File_Buf,"received: %s ",Rec_Buf);
    write_file(LOG_FILE, File_Buf);

    if ( strstr(Rec_Buf,POP_ERR_MSG) != NULL )  /* check for errors */
      pop_error();

    err=recv(sock,Rec_Buf,BUF_LEN,0);           /* retrieve the mail */
    sprintf(File_Buf,"%s",Rec_Buf);
    write_file(MAIL_FILE, File_Buf);
  }

  printf ("All done\n\n");

  /* send quit command then clean up */

  strcpy(Rec_Buf,"QUIT\r\n");
  err=send(sock,Rec_Buf,strlen(Rec_Buf),0);
  sprintf(File_Buf,"sent: %s", Rec_Buf);
  write_file(LOG_FILE, File_Buf);

  /* deallocate buffer memory */

  free(File_Buf);
  free(Rec_Buf);

  WSACleanup();
  return 0;
}

void handle_error(void)
{
  /*
   * Errors are handled by calling the WSAGetLastError routine which
   * will return the last error as one of the following. As we develop
   * this tutorial, we will go into much more detail on what they mean
   * and what caused them.
   */

  switch ( WSAGetLastError() )
  {
    case WSANOTINITIALISED :
      printf("Unable to initialise socket.\n");
    break;
    case WSAEAFNOSUPPORT :
      printf("The specified address family is not supported.\n");
    break;
    case WSAEADDRNOTAVAIL :
      printf("Specified address is not available from the local machine.\n");
    break;
    case WSAECONNREFUSED :
      printf("The attempt to connect was forcefully rejected.\n");
      break;
    case WSAEDESTADDRREQ :
      printf("address destination address is required.\n");
    break;
    case WSAEFAULT :
      printf("The namelen argument is incorrect.\n");
    break;
    case WSAEINVAL :
      printf("The socket is not already bound to an address.\n");
    break;
    case WSAEISCONN :
      printf("The socket is already connected.\n");
    break;
    case WSAEADDRINUSE :
      printf("The specified address is already in use.\n");
    break;
    case WSAEMFILE :
      printf("No more file descriptors are available.\n");
    break;
    case WSAENOBUFS :
      printf("No buffer space available. The socket cannot be created.\n");
    break;
    case WSAEPROTONOSUPPORT :
      printf("The specified protocol is not supported.\n");
      break;
    case WSAEPROTOTYPE :
      printf("The specified protocol is the wrong type for this socket.\n");
    break;
    case WSAENETUNREACH :
      printf("The network can't be reached from this host at this time.\n");
    break;
    case WSAENOTSOCK :
       printf("The descriptor is not a socket.\n");
    break;
    case WSAETIMEDOUT :
      printf("Attempt timed out without establishing a connection.\n");
    break;
    case WSAESOCKTNOSUPPORT :
       printf("Socket type is not supported in this address family.\n");
    break;
    case WSAENETDOWN :
      printf("Network subsystem failure.\n");
    break;
    case WSAHOST_NOT_FOUND :
      printf("Authoritative Answer Host not found.\n");
    break;
    case WSATRY_AGAIN :
      printf("Non-Authoritative Host not found or SERVERFAIL.\n");
     break;
    case WSANO_RECOVERY :
       printf("Non recoverable errors, FORMERR, REFUSED, NOTIMP.\n");
    break;
    case WSANO_DATA :
      printf("Valid name, no data record of requested type.\n");
    break;
      case WSAEINPROGRESS :
      printf("address blocking Windows Sockets operation is in progress.\n");
    break;
    case WSAEINTR :
      printf("The (blocking) call was canceled via WSACancelBlockingCall().\n");
    break;
    default :
      printf("Unknown error.\n");
     break;
  }

  WSACleanup();
//  exit(0);
}

void pop_error(void)
{
  printf("POP Error, Check the Log File for Details\r\n\r\n");

  WSACleanup();
//  exit(0);
}

void write_file(int file_type, char *p)
{
  const int LOG_FILE=1;                         /* write to log file */
  const int MAIL_FILE=2;                        /* write to mail file */
  const int SCREEN=4;                           /* display on screen */

  if( (file_type & LOG_FILE) != 0 )             /* add to logs */
  {
    FILE *fp=fopen("poplog.txt","a+");
    fprintf(fp,"%s\n",p);
    fclose(fp);
  }

  if( (file_type & MAIL_FILE) != 0 )            /* add to mailbox */
  {
    FILE *fp=fopen("mailbox.txt","a+");
    fprintf(fp,"%s\n",p);
    fclose(fp);
  }

  if ( (file_type & SCREEN) != 0 )              /* screen */
  {
    printf("%s\n",p);
  }
}

Log File Example after connecting to a POP Server:-

Pop Client Session started Tue Mar 28 22:27:06 2000.

Connecting to pop.freeuk.net

Connected to pop.freeuk.net

received: +OK CPOP v2.3 Ready for Action

sent: USER xxxxxxxx

received: +OK

xxxxxxxx

sent: PASS xxxxxxxx

received: +OK

xxxxxxxx

sent: STAT

received: +OK 1 7165

sent: RETR 1

received: +OK

1

sent: QUIT
#endif
      /*
*/