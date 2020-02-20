#include "define.h"

void writePush( FILE * ofp, int SEGNUMBER, int index ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "push %s\n", token );
	} else if ( isstdout  & !debug ) {
		if ( SEGNUMBER == CONST ) {
			fprintf( stdout, "push constant %d\n", index );
		} else if ( SEGNUMBER == ARG ) {
			fprintf( stdout, "push argument %d\n", index );
		} else if ( SEGNUMBER == LOCAL ) {
			fprintf( stdout, "push local %d\n", index );
		} else if ( SEGNUMBER == STATIC ) {
			fprintf( stdout, "push static %d\n", index );
		} else if ( SEGNUMBER == THIS ) {
			fprintf( stdout, "push this %d\n", index );
		} else if ( SEGNUMBER == THAT ) {
			fprintf( stdout, "push that %d\n", index );
		} else if ( SEGNUMBER == POINTER ) {
			fprintf( stdout, "push pointer %s\n", token );
		} else if ( SEGNUMBER == TEMP ) {
			fprintf( stdout, "push temp %d\n", index );
		}
	} else {
		if ( SEGNUMBER == CONST ) {
			fprintf( ofp, "push constant %d\n", index );
		} else if ( SEGNUMBER == ARG ) {
			fprintf( ofp, "push argument %d\n", index );
		} else if ( SEGNUMBER == LOCAL ) {
			fprintf( ofp, "push local %d\n", index );
		} else if ( SEGNUMBER == STATIC ) {
			fprintf( ofp, "push static %d\n", index );
		} else if ( SEGNUMBER == THIS ) {
			fprintf( ofp, "push this %d\n", index );
		} else if ( SEGNUMBER == THAT ) {
			fprintf( ofp, "push that %d\n", index );
		} else if ( SEGNUMBER == POINTER ) {
			fprintf( ofp, "push pointer %s\n", token );
		} else if ( SEGNUMBER == TEMP ) {
			fprintf( ofp, "push temp %d\n", index );
		}
	}	
}

void writePop( FILE * ofp, int SEGNUMBER, int index ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "pop %d\n", token );
	} else if ( isstdout & !debug ) {
		if ( SEGNUMBER == CONST ) {
			fprintf( stdout, "pop constant %d\n", index );
		} else if ( SEGNUMBER == ARG ) {
			fprintf( stdout, "pop argument %d\n", index );
		} else if ( SEGNUMBER == LOCAL ) {
			fprintf( stdout, "pop local %d\n", index );
		} else if ( SEGNUMBER == STATIC ) {
			fprintf( stdout, "pop static %d\n", index );
		} else if ( SEGNUMBER == THIS ) {
			fprintf( stdout, "pop this %d\n", index );
		} else if ( SEGNUMBER == THAT ) {
			fprintf( stdout, "pop that %d\n", index );
		} else if ( SEGNUMBER == POINTER ) {
			fprintf( stdout, "pop pointer %d\n", index );
		} else if ( SEGNUMBER == TEMP ) {
			fprintf( stdout, "pop temp %d\n", index );
		}
	} else {
		if ( SEGNUMBER == CONST ) {
			fprintf( ofp, "pop constant %d\n", index );
		} else if ( SEGNUMBER == ARG ) {
			fprintf( ofp, "pop argument %d\n", index );
		} else if ( SEGNUMBER == LOCAL ) {
			fprintf( ofp, "pop local %d\n", index );
		} else if ( SEGNUMBER == STATIC ) {
			fprintf( ofp, "pop static %d\n", index );
		} else if ( SEGNUMBER == THIS ) {
			fprintf( ofp, "pop this %d\n", index );
		} else if ( SEGNUMBER == THAT ) {
			fprintf( ofp, "pop that %d\n", index );
		} else if ( SEGNUMBER == POINTER ) {
			fprintf( ofp, "pop pointer %d\n", index );
		} else if ( SEGNUMBER == TEMP ) {
			fprintf( ofp, "pop temp %d\n", index );
		}
	}
}

void writeAritmetic( FILE * ofp, char command[256] ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "%d\n", token );
	} else if ( isstdout & !debug ) {
		if ( strcmp( command, "+" ) == 0 ) {
			fprintf( stdout, "add\n" );
		} else if ( strcmp( command, "-" ) == 0 ) {
			fprintf( stdout, "sub\n" );
		} else if ( strcmp( command, "-") == 0 ) {
			fprintf( stdout, "-%s\n", token );
		} else if ( strcmp( command, "~" ) == 0 ) {
			fprintf( stdout, "~%s\n", token );
		} else if ( strcmp( command, "=" ) == 0 ) {
			fprintf( stdout, "eq\n" );
		} else if ( strcmp( command, ">" ) == 0 ) {
			fprintf( stdout, "gt" );
		} else if ( strcmp( command, "<" ) == 0 ) {
			fprintf( stdout, "lt" );
		} else if ( strcmp( command, "&" ) == 0 ) {
			fprintf( stdout, "&\n" );
		} else if ( strcmp( command, "|" ) == 0 ) {
			fprintf( stdout, "|" );
		} else if ( strcmp( command, "!" ) == 0 ) {
			fprintf( stdout, "!" );
		}
	} else {
		if ( strcmp( command, "+" ) == 0 ) {
			fprintf( ofp, "add\n" );
		} else if ( strcmp( command, "-" ) == 0 ) {
			fprintf( ofp, "sub\n" );
		} else if ( strcmp( command, "-") == 0 ) {
			fprintf( ofp, "-%s\n", token );
		} else if ( strcmp( command, "~" ) == 0 ) {
			fprintf( ofp, "~%s\n", token );
		} else if ( strcmp( command, "=" ) == 0 ) {
			fprintf( ofp, "eq\n" );
		} else if ( strcmp( command, ">" ) == 0 ) {
			fprintf( ofp, "gt" );
		} else if ( strcmp( command, "<" ) == 0 ) {
			fprintf( ofp, "lt" );
		} else if ( strcmp( command, "&" ) == 0 ) {
			fprintf( ofp, "&\n" );
		} else if ( strcmp( command, "|" ) == 0 ) {
			fprintf( ofp, "|" );
		} else if ( strcmp( command, "!" ) == 0 ) {
			fprintf( ofp, "!" );
		}
	}
}

void writeLabel( FILE * ofp, char labelname[256] ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "label %s\n", labelname );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "label %s\n", labelname );
	} else {
		fprintf( ofp, "label %s\n", labelname );
	}
}

void writeGoto( FILE * ofp, char labelname[256] ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "goto %s\n", labelname );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "goto %s\n", labelname );
	} else {
		fprintf( ofp, "goto %s\n", labelname );
	}
}

void writeIf( FILE * ofp, char labelname[256] ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "if-goto %s\n", labelname );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "if-goto %s\n", labelname );
	} else {
		fprintf( ofp, "if-goto %s\n", labelname );
	}
}

void writeCall( FILE * ofp, char func[256], int nargs ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "call %s %n\n", func, nargs );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "call %s %n\n", func, nargs );
	} else {
		fprintf( ofp, "call %s %n\n", func, nargs );
	}
}

void writeFunction( FILE * ofp, char func[256], int nlocals ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "call %s %n\n", func, nargs );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "call %s %n\n", func, nargs );
	} else {
		fprintf( ofp, "call %s %n\n", func, nargs );
	}

}
