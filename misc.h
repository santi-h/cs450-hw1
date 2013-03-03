#ifndef MISC_H
#define MISC_H

#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <netdb.h>
#include <assert.h>
#include <string.h>

/****************************************************************************************
* STRUCTS 
****************************************************************************************/
struct byte_vector
{
	char* array;
	int size; 
};

/****************************************************************************************
* FUNCTIONS
****************************************************************************************/
void clean_vector( struct byte_vector*);
struct byte_vector make_vector( int size);
char* trim_str( char* str);
void finish( const char* msg, int stat);
struct addrinfo* Getaddrinfo(	const char* node,
								const char *service,
								const struct addrinfo *hints);
void* Calloc( size_t num, size_t size);
void* Malloc( size_t size);


#endif //MISC_H


