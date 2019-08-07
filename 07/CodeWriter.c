#include "define.h"

void setFileName() {
	outputfp = fopen( outputfilename, "w" );
}

void writeArithmetic( char * command ) {

	char convstr[256];

	// 各算術用VMコマンドを所望の動作を行うhackアセンブラに
	// 変換suru
	if ( strcmp( command, "add" ) == 0 ) {
		callAddFunction();
	} else if ( strcmp( command, "sub" ) == 0 ) {
		// fprintf( outputfp, "M=D-M\n" );
		callSubFunction();
	} else if ( strcmp( command, "neg" ) == 0 ) {
		// fprintf( outputfp, "-M\n" );
		callNegFunction();
	} else if ( strcmp( command, "eq" ) == 0 ) { 
		// fprintf( outputfp, "JLE\n" );
		callEqFunction();
	} else if ( strcmp( command, "gt" ) == 0 ) {
		// fprintf( outputfp, "JGT\n" );
		callGtFunction();
	} else if ( strcmp( command, "lt" ) == 0 ) {
		// fprintf( outputfp, "JLT\n" );
		callLtFunction();
	} else if ( strcmp( command, "and" ) == 0 ) {
		// fprintf( outputfp, "D&M\n" );
		callAndFunction();
	} else if ( strcmp( command, "or" ) == 0 ) {
		// fprintf( outputfp, "D|M\n" );
		callOrFunction();
	} else if ( strcmp( command, "not" ) == 0 ) {
		// fprintf( outputfp, "!M\n" );
		callNotFunction();
	}
}

void writePushPop( int command, char * segment, int index ) {
	if ( command == C_POP ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			callPopLocalFunction( index );	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			callPopArgumentFunction( index );
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			callPopThisFunction( index );
		} else if ( strcmp( segment, "that" ) == 0 ) {
			callPopThatFunction( index );
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			;
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			;
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			callPopConstantFunction( index );
		} else if ( strcmp( segment, "static" ) == 0 ) {
			fprintf( outputfp, "@xxx.%d\n", index );
			fprintf( outputfp, "D=M\n" );
		}
	} else if ( command == C_PUSH ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			callPushLocalFunction( index );	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			callPushArgumentFunction( index );
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			callPushThisFunction( index );
		} else if ( strcmp( segment, "that" ) == 0 ) {
			callPushThatFunction( index );
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			;
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			;
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			callPushConstantFunction( index );
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

