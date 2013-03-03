#include "misc.h"

/****************************************************************************************
* Prints out msg, and exits with status stat
****************************************************************************************/
void finish( const char* msg, int stat)
{
	printf("%s\n", msg);
	exit(stat);
}

/****************************************************************************************
* reimplementation of getaddrinfo() that exits on error
****************************************************************************************/
struct addrinfo* Getaddrinfo(	const char* node,
								const char *service,
								const struct addrinfo *hints)
{
	int int_res = 0;
	struct addrinfo* ret;
	if( (int_res = getaddrinfo( node, service, hints, &ret)) != 0)
	{
		printf("getaddrinfo() error occurred: ");
		finish( gai_strerror( int_res), 1);
	}
	
	return ret;
}

/****************************************************************************************
* frees memory allocated in vector and sets size to 0
****************************************************************************************/
void clean_vector( struct byte_vector* vector)
{
	if( vector->size)
		free( vector->array);

	vector->size = 0;
}

/****************************************************************************************
* creates/allocates a struct vector of size size and returns it
****************************************************************************************/
struct byte_vector make_vector( int size)
{
	struct byte_vector ret;
	ret.array = (char*)Calloc( size, 1);
	ret.size = size;
	return ret;
}

/****************************************************************************************
* reimplementation of malloc() that exits on error 
****************************************************************************************/
void* Malloc( size_t size)
{
	void* ret = malloc( size);
	assert( ret);
	return ret;
}

/****************************************************************************************
* reimplementation of calloc() that exits on error
****************************************************************************************/
void* Calloc( size_t num, size_t size)
{
	void* ret = calloc( num, size);
	assert( ret);
	return ret;
}

/****************************************************************************************
* takes away spaces at the end and at the beginning of str
****************************************************************************************/
char* trim_str( char* str)
{
	int len = 0;
	if( str) len = strlen( str);

	if( len)
	{
		int i, j;
		int t=0;

		for( i=0; i<len && str[i]==' '; i++);
		for( j=len-1; j>=0 && str[j]==' '; j--);
		
		if( i<=j)
		{
			for( t=0; i<=j; t++, i++) str[t] = str[i];
		}

		str[t] = '\0';
		
	}
	
	return str;
} 

/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
/****************************************************************************************
* 
****************************************************************************************/
