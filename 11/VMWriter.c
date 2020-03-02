#include "define.h"

void writePush( FILE * ofp, int SEGNUMBER, int index ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "push %s\n", token );
	} else if ( isstdout  & !debug ) {
		if ( SEGNUMBER == VM_CONST ) {
			fprintf( stdout, "push constant %s\n", token );
		} else if ( SEGNUMBER == VM_ARG ) {
			fprintf( stdout, "push argument %d\n", index );
		} else if ( SEGNUMBER == VM_LOCAL ) {
			fprintf( stdout, "push local %d\n", index );
		} else if ( SEGNUMBER == VM_STATIC ) {
			fprintf( stdout, "push static %d\n", index );
		} else if ( SEGNUMBER == VM_THIS ) {
			fprintf( stdout, "push this %d\n", index );
		} else if ( SEGNUMBER == VM_THAT ) {
			fprintf( stdout, "push that %d\n", index );
		} else if ( SEGNUMBER == VM_POINTER ) {
			fprintf( stdout, "push pointer %s\n", token );
		} else if ( SEGNUMBER == VM_TEMP ) {
			fprintf( stdout, "push temp %d\n", index );
		}
	} else {
		if ( SEGNUMBER == VM_CONST ) {
			fprintf( ofp, "push constant %s\n", token );
		} else if ( SEGNUMBER == VM_ARG ) {
			fprintf( ofp, "push argument %d\n", index );
		} else if ( SEGNUMBER == VM_LOCAL ) {
			fprintf( ofp, "push local %d\n", index );
		} else if ( SEGNUMBER == VM_STATIC ) {
			fprintf( ofp, "push static %d\n", index );
		} else if ( SEGNUMBER == VM_THIS ) {
			fprintf( ofp, "push this %d\n", index );
		} else if ( SEGNUMBER == VM_THAT ) {
			fprintf( ofp, "push that %d\n", index );
		} else if ( SEGNUMBER == VM_POINTER ) {
			fprintf( ofp, "push pointer %s\n", token );
		} else if ( SEGNUMBER == VM_TEMP ) {
			fprintf( ofp, "push temp %d\n", index );
		}
	}	
}

void writePop( FILE * ofp, int SEGNUMBER, int index ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "pop %s\n", token );
	} else if ( isstdout & !debug ) {
		if ( SEGNUMBER == VM_CONST ) {
			fprintf( stdout, "pop constant %d\n", index );
		} else if ( SEGNUMBER == VM_ARG ) {
			fprintf( stdout, "pop argument %d\n", index );
		} else if ( SEGNUMBER == VM_LOCAL ) {
			fprintf( stdout, "pop local %d\n", index );
		} else if ( SEGNUMBER == VM_STATIC ) {
			fprintf( stdout, "pop static %d\n", index );
		} else if ( SEGNUMBER == VM_THIS ) {
			fprintf( stdout, "pop this %d\n", index );
		} else if ( SEGNUMBER == VM_THAT ) {
			fprintf( stdout, "pop that %d\n", index );
		} else if ( SEGNUMBER == VM_POINTER ) {
			fprintf( stdout, "pop pointer %d\n", index );
		} else if ( SEGNUMBER == VM_TEMP ) {
			fprintf( stdout, "pop temp %d\n", index );
		}
	} else {
		if ( SEGNUMBER == VM_CONST ) {
			fprintf( ofp, "pop constant %d\n", index );
		} else if ( SEGNUMBER == VM_ARG ) {
			fprintf( ofp, "pop argument %d\n", index );
		} else if ( SEGNUMBER == VM_LOCAL ) {
			fprintf( ofp, "pop local %d\n", index );
		} else if ( SEGNUMBER == VM_STATIC ) {
			fprintf( ofp, "pop static %d\n", index );
		} else if ( SEGNUMBER == VM_THIS ) {
			fprintf( ofp, "pop this %d\n", index );
		} else if ( SEGNUMBER == VM_THAT ) {
			fprintf( ofp, "pop that %d\n", index );
		} else if ( SEGNUMBER == VM_POINTER ) {
			fprintf( ofp, "pop pointer %d\n", index );
		} else if ( SEGNUMBER == VM_TEMP ) {
			fprintf( ofp, "pop temp %d\n", index );
		}
	}
}

void writeAritmetic( FILE * ofp, char command[256] ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "%s\n", token );
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
		} else if ( strcmp( command, "*" ) == 0 ) {
			fprintf( stdout, "call Math.multiply 2\n" );
		} else if ( strcmp( command, "/" ) == 0 ) {
			fprintf( stdout, "call Math.divide 2\n" );
		} else if ( strcmp( command, "neg" ) == 0 ) {
			fprintf( stdout, "neg\n" );
		}
	} else {
		if ( strcmp( command, "+" ) == 0 ) {
			fprintf( ofp, "add\n" );
		} else if ( strcmp( command, "-" ) == 0 ) {
			fprintf( ofp, "sub\n" );
		} else if ( strcmp( command, "-") == 0 ) {
			fprintf( ofp, "neg\n" );
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
		} else if ( strcmp( command, "*" ) == 0 ) {
			fprintf( ofp, "call Math.multiply 2\n" );
		} else if ( strcmp( command, "/" ) == 0 ) {
			fprintf( ofp, "call Math.divide 2\n" );
		} else if ( strcmp( command, "neg" ) == 0 ) {
			fprintf( ofp, "neg\n" );
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
		fprintf( stdout, "call %s %d\n", func, nargs );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "call %s %d\n", func, nargs );
	} else {
		fprintf( ofp, "call %s %d\n", func, nargs );
	}
}

void writeFunction( FILE * ofp, char func[256], int nlocals ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "function %s %d\n", func, nlocals );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "function %s %d\n", func, nlocals );
	} else {
		fprintf( ofp, "function %s %d\n", func, nlocals );
	}
}

void writeReturn( FILE * ofp ) {
	if ( isstdout & debug ) {
		fprintf( stdout, "return\n" );
	} else if ( isstdout & !debug ) {
		fprintf( stdout, "return\n" );
	} else {
		fprintf( ofp, "return\n" );
	}
}
