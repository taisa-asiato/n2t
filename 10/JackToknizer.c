#include "define.h" 

void jack_tokenizer_main( FILE * ifp, FILE * ofp  ) {
	char current_line[256];
	char * cp;
	int type_of_token;
	int type_of_keyword;
	char symbol_string[256];
	char identifier_string[256];

	while ( fgets( streamline, ( sizeof( streamline )/sizeof( char ) ), ifp ) ) {
		strcpy( current_line, streamline );

		fprintf( stdout, "%s", current_line );
		cp = strtok( current_line, " " );

		while ( has_more_tokens( cp ) ) {
			fprintf( stdout, "%s\n", cp  );
			advance( cp );

			type_of_token = token_type( token );

			if ( type_of_token == KEYWORD ) {
				type_of_keyword = keyword( token );
			} else if ( type_of_token == SYMBOL ) {
				symbol( symbol_string );
			} else if ( type_of_token == IDENTIFIER ) {
				identifier( identifier_string );
			} else if ( type_of_token == INT_CONST ) {
				int_val( token );
			} else if ( type_of_token == STRING_CONST ) {
				string_val( token );
			}
			cp = strtok( NULL, " " );
		}
	} 
}

bool has_more_tokens( char * istoken ) {

	if ( istoken != NULL ) {
		return true;
	}

	return false;
}

void advance( char * cp ) {
	// 次のトークンに進める
	strcpy( token, cp );
}

int token_type( char current[256] ) {
	
	if ( is_keyword( current ) ) {
		return KEYWORD;
	} else if ( is_symbol( current ) ) {
		return SYMBOL;
	} else if ( is_integer_constant( current ) ) {
		return INT_CONST;
	} else if ( is_string_constant( current ) ) {
		return STRING_CONST;
	} else if ( is_identifier( current ) )  {
		return IDENTIFIER;
	}
	
	return -1;
}

bool is_keyword( char c_token[256] ) {
	if ( 
		strcmp( c_token, "class" ) == 0 || strcmp( c_token, "constructor" ) == 0 || 
		strcmp( c_token, "function" ) == 0 || strcmp( c_token, "method" ) == 0  || 
		strcmp( c_token, "field" ) == 0 || strcmp( c_token, "static" ) == 0 || 
		strcmp( c_token, "var" ) == 0 || strcmp( c_token, "int" ) == 0 || 
		strcmp( c_token, "char" ) == 0 || strcmp( c_token, "boolean" ) == 0 || 
		strcmp( c_token, "void" ) == 0 || strcmp( c_token, "true" ) == 0 || 
		strcmp( c_token, "false" ) == 0 || strcmp( c_token, "null") == 0 || 
		strcmp( c_token, "this" ) == 0 || strcmp( c_token, "let" ) == 0  || 
		strcmp( c_token, "do" ) == 0 || strcmp( c_token, "if" ) == 0 || 
		strcmp( c_token, "else" ) == 0 || strcmp( c_token, "while" ) == 0 || 
		strcmp( c_token, "return" ) == 0 ) {
		return true;
	}
	return false;
}

bool is_symbol( char c_token[256] ) {
	if (
		strcmp( c_token, "{" ) || strcmp( c_token, "}" ) || strcmp( c_token, "(" ) ||
		strcmp( c_token, ")" ) || strcmp( c_token, "[" ) || strcmp( c_token, "]" ) ||
		strcmp( c_token, "." ) || strcmp( c_token, "," ) || strcmp( c_token, ";" ) || 
		strcmp( c_token, "+" ) || strcmp( c_token, "-" ) || strcmp( c_token, "*" ) || 
		strcmp( c_token, "/" ) || strcmp( c_token, "&" ) || strcmp( c_token, "|" ) ||
		strcmp( c_token, "<" ) || strcmp( c_token, ">" ) || strcmp( c_token, "=" ) ||
		strcmp( c_token, "~" ) ) {
		return true;
	}
	return false;
}

bool is_integer_constant( char c_token[256] ) {
	int num;
	if ( ( num = atoi( c_token ) ) ) {
		if ( num >= 0 || num <= 32767 ) {
			return true;
		}
	}
	return false;
}

bool is_string_constant( char c_token[256] ) {
	int length = strlen( c_token );
	if ( length ) {
		return true;	
	}
	return false;
}

int keyword( char c_token[256] ) {
	if ( strcmp( c_token, "class" ) == 0 ) {
		return CLASS;
	} else if ( strcmp( c_token, "method" ) == 0 ) {
		return METHOD;
	} else if ( strcmp( c_token, "function" ) == 0 ) {
		return FUNCTION;;
	} else if ( strcmp( c_token, "constructor" ) == 0 ) {
		return CONSTRUCTOR;
	} else if ( strcmp( c_token, "int" ) == 0 ) {
		return INT;
	} else if ( strcmp( c_token, "boolean" ) == 0 ) {
		return BOOLEAN;
	} else if ( strcmp( c_token, "char" ) == 0 ) {
		return CHAR;
	} else if ( strcmp( c_token, "void" ) == 0 ) {
		return VOID;
	} else if ( strcmp( c_token, "var" ) == 0 ) {
		return VAR;
	} else if ( strcmp( c_token, "static" ) == 0 ) {
		return STATIC;
	} else if ( strcmp( c_token, "field" ) == 0 ) {
		return FIELD;
	} else if ( strcmp( c_token, "let" ) == 0 ) { 
		return LET;
	} else if ( strcmp( c_token, "do" ) == 0 ) {
		return DO;
	} else if ( strcmp( c_token, "if" ) == 0 ) {
		return IF;
	} else if ( strcmp( c_token, "else" ) == 0 ) {
		return ELSE;
	} else if ( strcmp( c_token, "while" ) == 0 ) {
		return WHILE;
	} else if ( strcmp( c_token, "return" ) == 0 ) {
		return RETURN;
	} else if ( strcmp( c_token, "true" ) == 0 ) {
		return TRUE;
	} else if ( strcmp( c_token, "false" ) == 0 ) {
		return FALSE;
	} else if ( strcmp( c_token, "null" ) == 0 ) {
		return exNULL;
	} else if ( strcmp( c_token, "this" ) == 0 ) {
		return THIS;
	}

	return -1;
}

void symbol( char symbol_string[256] ) {
	strcpy( symbol_string, token );
}

void identifier( char identifier_string[256] ) {
	strcpy( identifier_string, token );
}

int int_val( char intval_string[256] ) {
	return atoi( intval_string );
}

void string_val( char val_string[256] ) {
	strcpy( val_string, token );
}

bool is_identifier( char val_string[256] ) {
	if ( strlen( val_string ) > 0  ) {
		return true;
	}
	return false;
}
