#include "define.h"

void setFileName( char * outputfilename ) {
	outputfp = fopen( outputfilename, "w" );
}

void writeArithmetic( char * command ) {

	char convstr[256];

	if ( strcmp( command, "add" ) == 0 ) {
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

void writePushPop( int command, char * segment, int index ) {
	if ( command == C_POP ) {
		;
	} else if ( command == C_PUSH ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			;	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			;
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			;
		} else if ( strcmp( segment, "that" ) == 0 ) {
			;
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			;
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			;
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			// constantはindexの値を定数値のindexとして扱うのみ
			fprintf( outputfp, "@%d\n", index );
		} else if ( strcmp( segment, "static" ) == 0 ) {
			fprintf( outputfp, "@xxx.%d\n", index );
			fprintf( outputfp, "D=M\n" );
		}
	} else {
		fprintf( stdout, "ERROR: command is invalid value\n" );
	}
}

void close() {
	fclose( outputfp );
}
