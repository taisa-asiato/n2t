#include "define.h"


void compile_Class( char * token_pointer ) {
	char * cp;

	cp = advance();
	
	if ( cp == identifier ) {
		
	} else {
		fprintf( stdout, "ERROR:Next keyword of Class is expected identifier, classname\n" );
		return false;
	}

	cp = advance();

	if ( cp[0] == '{' ) {
		;
	} else {
		fprintf( stdout, "ERROR:Next Keyword of classname is expected symbol, {\n" );
		return false;
	}

	cp = advance();

	if ( class_var_dec() ) {
	
	} else {
		fprintf( stdout, "ERROR:Next Keyword of symbol, classVarDec\n" );
		return false;
	}
}

void class_var_dec( char * cp )  {
	
	if ( strcmp( cp, "static" ) == 0 || strcmp( cp, "field" ) == 0 ) {
		;
	} else {
		fprintf( stdout, "ERROR:\n" );
	}

	cp = advance();
}

void type( char * cp ) {
	
	if ( 
		strcmp( cp, "int" ) == 0 || strcmp( cp, "char" ) == 0 || 
		strcmp( cp, "boolean" ) == 0 || cp == identifier ) {
		
	}
}
