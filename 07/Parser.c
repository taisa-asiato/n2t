#include "define.h"

FILE * fp;
char filename[256];
char line[256];


int main( int argv, char ** argc ) {

	strcpy( filename, argc[1] );
	fprintf( stdout, "%s\n", filename );
	ParseMain();
	return 0;	
}

void ParseMain() {
	fp = fopen( filename, "r" ); 

	while ( hasMoreCommands() ) {
		fprintf( stdout, "%s", line );	
	}

	fclose( fp );
}

bool hasMoreCommands() {
		
	if ( fgets( line, sizeof( line ) / sizeof( char ), fp ) ) {
		return true;
	}
	
	return false;
}
