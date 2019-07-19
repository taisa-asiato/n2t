#include "define.h"

void setFileName( char ** outputfilename ) {
	outputfp = fopen( outputfilename, "w" );

}

void writeArithmetic( char ** command ) {

	char convstr[256];

	if ( strcmp( comamnd, "add" ) == 0 ) {
		fprintf( outputfp, "D+A\n" );
	} else if ( strcmp( command, "sub" ) == 0 ) {
		fprintf( outputfp, "D-A\n" );
	} else if ( strcmp( command, "neg" ) == 0 ) {
		fprintf( outputfp, "-A\n" );
	} else if ( strcmp( command, "eq" ) == 0 ) { 
		fprintf( outputfp, "JLE\n" );
	} else if ( strcmp( command, "gt" ) == 0 ) {
		fprintf( outputfp, "JGT\n" );
	} else if ( strcmp( command, "lt" ) == 0 ) {
		fprintf( outputfp, "JLT\n" );
	} else if ( strcmp( command, "and" ) == 0 ) {
		fprintf( outputfp, "D&A\n" );
	} else if ( strcmp( command, "or" ) == 0 ) {
		fprintf( outputfp, "D|A\n" );
	} else if ( strcmp( command, "not" ) == 0 ) {
		fprintf( outputfp, "!A\n" );
	}
}

void close() {
	fclose( outputfp );
}
