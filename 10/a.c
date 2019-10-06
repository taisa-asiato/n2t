#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main( int argc, char ** argv ) {

	char str[256];
	FILE * fp = fopen( argv[1], "r" );
	char * cp;

	fprintf( stdout, "%s\n", argv[1] );
	while ( fgets( str, sizeof( str ) / sizeof( char ), fp ) ) {
		if ( str[0] != '\r' ) {	
			cp = strtok( str, "\t" );
			if ( strcmp( cp, "\r\n" ) != 0 ) {
				fprintf( stdout, "[%d] %s", str[0], str );
			}
		}
	}
	fclose( fp );
	return 0;
}
