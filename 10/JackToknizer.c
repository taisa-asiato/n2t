#include "define.h" 

void jack_tokenizer_main( FILE * ifp, FILE * ofp  ) {
	char current_line[256];
	char * cp;
	int type_of_token;
	int type_of_keyword;
	char symbol_string[256];
	char identifier_string[256];
	char string_const[256];

	while ( fgets( streamline, ( sizeof( streamline )/sizeof( char ) ), ifp ) ) {
		strcpy( current_line, streamline );

		// fprintf( stdout, "%s", current_line );
		cp = strtok( current_line, " \t" );
		if ( cp != NULL && strcmp( cp, "\r\n" ) != 0 ) {
			while ( has_more_tokens( cp ) ) {
				advance( cp );
				type_of_token = token_type( token );

				if ( strcmp( cp, "//" ) != 0 && strcmp( cp, "/*" ) != 0 && strcmp( cp, "/**" ) != 0 ) {
					fprintf( stdout, "[%s]\n", cp  );
					if ( type_of_token == KEYWORD ) {
						fprintf( stdout, "\tcalling keyword function\n" );
						type_of_keyword = keyword( token );
					} else if ( type_of_token == SYMBOL ) {
						fprintf( stdout, "\tcalling symbol function\n" );
						symbol( symbol_string );
					} else if ( type_of_token == IDENTIFIER ) {
						fprintf( stdout, "\tcalling identifier function\n" );
						identifier( identifier_string );
					} else if ( type_of_token == INT_CONST ) {
						fprintf( stdout, "\tcalling int_const function\n" );
						int_val( token );
					} else if ( type_of_token == STRING_CONST ) {
						fprintf( stdout, "\tcalling string_const function\n" );
						string_val( string_const );
					} else {
						fprintf( stdout, "No token, if reach here, this means error\n" );
					}
				} else {
					break;
				}

				fprintf( stdout, "\t\t\tNext Round\n" ); 
				cp = strtok( NULL, " \t\r\n" );
				fprintf( stdout, "\t\t\tCheck\n" );
			}
		}
	} 
}

bool has_more_tokens( char * istoken ) {
	if ( istoken == NULL ) {
		return false;
	}
	if ( istoken[0] != '\0' ) {
		return true;
	}
	fprintf( stdout, "target pointer is null value\n" );
	return false;
}

void advance( char * cp ) {
	// 次のトークンに進める
	strcpy( token, cp );
}

int token_type( char current[256] ) {
	if ( is_keyword( current ) ) {
		fprintf( stdout, "keyword\n" );
		return KEYWORD;
	} else if ( is_symbol( current ) ) {
		fprintf( stdout, "symbol\n" );
		return SYMBOL;
	} else if ( is_integer_constant( current ) ) {
		fprintf( stdout, "int_const\n" );
		return INT_CONST;
	} else if ( is_string_constant( current ) ) {
		fprintf( stdout, "string_const\n" );
		return STRING_CONST;
	} else if ( is_identifier( current ) )  {
		fprintf( stdout, "identifier\n" );
		return IDENTIFIER;
	}
	return -1;
}

bool is_keyword( char c_token[256] ) {
	if ( 
		strcmp( c_token, "class" ) == 0 	|| strcmp( c_token, "constructor" ) == 0 || 
		strcmp( c_token, "function" ) == 0 	|| strcmp( c_token, "method" ) == 0  || 
		strcmp( c_token, "field" ) == 0 	|| strcmp( c_token, "static" ) == 0 || 
		strcmp( c_token, "var" ) == 0		|| strcmp( c_token, "int" ) == 0 || 
		strcmp( c_token, "char" ) == 0 		|| strcmp( c_token, "boolean" ) == 0 || 
		strcmp( c_token, "void" ) == 0 		|| strcmp( c_token, "true" ) == 0 || 
		strcmp( c_token, "false" ) == 0 	|| strcmp( c_token, "null") == 0 || 
		strcmp( c_token, "this" ) == 0 		|| strcmp( c_token, "let" ) == 0  || 
		strcmp( c_token, "do" ) == 0 		|| strcmp( c_token, "if" ) == 0 || 
		strcmp( c_token, "else" ) == 0 		|| strcmp( c_token, "while" ) == 0 || 
		strcmp( c_token, "return" ) == 0 ) {
		return true;
	}
	return false;
}

bool is_symbol( char c_token[256] ) {
	if (
		strcmp( c_token, "{" ) == 0 || strcmp( c_token, "}" ) == 0 || strcmp( c_token, "(" ) == 0 ||
		strcmp( c_token, ")" ) == 0 || strcmp( c_token, "[" ) == 0 || strcmp( c_token, "]" ) == 0 ||
		strcmp( c_token, "." ) == 0 || strcmp( c_token, "," ) == 0 || strcmp( c_token, ";" ) == 0 || 
		strcmp( c_token, "+" ) == 0 || strcmp( c_token, "-" ) == 0 || strcmp( c_token, "*" ) == 0 || 
		strcmp( c_token, "/" ) == 0 || strcmp( c_token, "&" ) == 0 || strcmp( c_token, "|" ) == 0 ||
		strcmp( c_token, "<" ) == 0 || strcmp( c_token, ">" ) == 0 || strcmp( c_token, "=" ) == 0 ||
		strcmp( c_token, "~" ) == 0 ) {
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
	int i = 0;
	int str_flag = 0;
	
	while ( c_token[i] != '\0' ) {
		if ( isalpha( c_token[i] ) ) {
			i++;
		} else {
			c_token[i] = '\0';
			str_flag = 1;
			break;
		}
	}

	if ( str_flag == 1 ) {
		return false;
	}

	return true;
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

void symbol( char * symbol_string ) {
	strcpy( symbol_string, token );
}

void identifier( char identifier_string[256] ) {
	int i = 0;

	while ( token[i] != '\0' )  {
		if ( isalpha( token[i] ) ) {
			identifier_string[i] = token[i];
			i++;
		} else {
			identifier_string[i] = '\0';
			break;
		}
	}
}

int int_val( char intval_string[256] ) {
	return atoi( intval_string );
}

void string_val( char val_string[256] ) {
	strcpy( val_string, token );
}

bool is_identifier( char val_string[256] ) {
	int length = strlen( val_string );
	int i = 0;

	if ( val_string[0] >= '0' && val_string[0] <= '9' ) {
		return false;
	}

	while ( i < length  ) {
		if ( isalpha( val_string[i] ) || val_string[i] == '_' || ( val_string[i] >= '0' && val_string[i] <= '9' ) ) {
			i++;
		} else {
			return false;
		}
	}
	return true;
}
