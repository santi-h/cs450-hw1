#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include "misc.h"

/****************************************************************************************
* Receives the response of the server and creates the file
****************************************************************************************/
void recv_response( int sock, const char* obj)
{
	const int BUF_SIZE = 150;
	char buf[BUF_SIZE];	
	int recvres;
	FILE* file = 0;
	int t = 0; // 4: entity body reached
	int s = 0; // 0: result code not reached yet. -1: result code asserted
	int i;

	while( (recvres = recv( sock, buf, BUF_SIZE, 0)) > 0) // while socket is still connected
	{
		for( i=0; i<recvres && t<4; i++)
		{
			if( buf[ i] == '\r' && (t==0 || t==2))
				t++;
			else if( buf[ i] == '\n' && (t==1 || t==3))
				t++;
			else
			{ 
				t = 0;
				if( buf[ i] == ' ' && s>-1)
				{
					if( !s)
						s = i+1;
					else
					{
						if( atoi(&buf[s])!=200)
							finish("Response contains an error. The file may not exist.",1);
						s = -1;
					}
				}
			}
		}
		
		if( t>=4)
		{
			if( !file)
			{
				file = fopen( obj, "w");
				if( !file) finish( "Could not create file", 1);
			}
			
			for( ; i<recvres; i++)fputc( buf[ i], file);
		}
	}
	
	if( recvres == -1)
	{
		printf( "Error at receiving (%d): %s\n", errno, strerror( errno));
		exit( 1);
	}

	if( file)
		if( fclose( file)) finish("Error when closing the file", 1);

}

/****************************************************************************************
* 
****************************************************************************************/
struct addrinfo* getaddr( const char* server, const char* service, int fam)
{
	struct addrinfo hints;
	memset( &hints, 0, sizeof( struct addrinfo));
	hints.ai_family = fam; //AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	return Getaddrinfo( server, service, &hints);
}

/****************************************************************************************
* Get the host, filepath, and filename from the url
****************************************************************************************/
void get_url_file( const char* arg, char* url, char* filepath, char* filename, int buf)
{
	int arglen = strlen( arg);
	int urlstart, urlend, urllen, filelen, i, j;
	char found_start = 0;

	for( urlstart=1; urlstart<arglen && !found_start; urlstart++)
		if( arg[urlstart-1] == '/' && arg[urlstart] == '/') found_start = 1;

	if( !found_start) urlstart = 0;

	for( urlend = urlstart; urlend<arglen && arg[urlend]!='/'; urlend++);
	
	urllen = urlend - urlstart;
	filelen = (urlend == arglen) ? (0) : (arglen-urlend-1) ;
	
	for( i=0; i<urllen; i++) url[i] = arg[urlstart++];
	url[ i] = 0;
	
	urlstart++; filepath[ 0] = '/';
	for( i=0; i<filelen; i++) filepath[i+1] = arg[urlstart++];
	filepath[ i+1] = 0;

	for( j=0, i=0; i<=filelen; i++)
	{
		if( filepath[ i] == '/') j = 0; else filename[ j++] = filepath[ i];
	}
	filename[ j] = 0;
}

/****************************************************************************************
* sends the object request to the server
****************************************************************************************/
void send_request( int sock, const char* url, const char* filepath)
{
	char* req = (char*)malloc( 50 + strlen( url) + strlen( filepath));
	req[0] = 0;
	strcat( req, "GET "); strcat( req, filepath); strcat( req, " HTTP/1.0\r\n");
	strcat( req, "Host: "); strcat( req, url); strcat( req, "\r\n\r\n");

	int msglen = strlen( req);
	int bsent = 0;
	int bsent_i;
	while( bsent<msglen)
	{
		bsent_i = send( sock, &req[bsent], msglen, 0);
		if( bsent_i == -1)
		{
			printf( "send() failed: \"%s\"\n", strerror( errno));
			exit( 1);
		}

		bsent += bsent_i;
	}

	free( req);
}

/****************************************************************************************
* 
****************************************************************************************/
int main(int argc, char* argv[])
{
	const int BUF_SIZE = 50;
	char url[BUF_SIZE];
	char filepath[BUF_SIZE];
	char filename[BUF_SIZE];
	int sock;
	struct addrinfo* addrinf;
	
	//* GET url, filepath, AND filename
	if( argc>1)
		get_url_file( argv[1], url, filepath, filename, BUF_SIZE);
	else
		finish("Too few arguments\n", 1);
	
	if( !strlen( filename)) strcpy( filename, "index.html");	
	printf( "url: [%s]\nfilepath: [%s]\nfilename: [%s]\n\n", url, filepath, filename);
	//*/
	
	//* GET REMOTE addrinf
	addrinf = getaddr( url, "80", AF_UNSPEC);
	if( !addrinf) finish( "getaddrinfo() returned no results\n", 1);
	//*/
	
	//* CREATE AND CONNECT THE SOCKET
	sock = socket( addrinf->ai_family, SOCK_STREAM, 0);
	if( sock == -1)
	{
		printf( "couldn't create the socket, errno(%d): %s\n", errno, strerror( errno));
		exit(1);
	}
	if( connect( sock, addrinf->ai_addr, addrinf->ai_addrlen) != 0)
	{
		printf( "couldn't connect, errno(%d): %s\n", errno, strerror( errno));
		exit(1);
	}//*/

	//* DO IT
	send_request( sock, url, filepath);
	recv_response( sock, filename);
	//*/

	//* CLEAN UP AND EXIT
	assert( shutdown( sock, SHUT_RDWR)==0);
	//*/

	return 0;
}


