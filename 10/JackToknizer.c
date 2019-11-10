#include "define.h" 

void jack_tokenizer_main( FILE * ifp, FILE * ofp  ) {
	char current_line[256];
	char * cp;
	int type_of_token;
	int int_num;
	int type_of_keyword;
	char symbol_string[256];
	char identifier_string[256];
	char string_const[256];
	char str_c;
	int loopcounter = 0;


	// fprintf( stdout, "%s", current_line );
	while ( has_more_tokens( ifp ) ) {
		// fprintf( stdout, "input file\n" );
		// fprintf( stdout, "%c\n", *cp );
		// cp++;
		advance( ifp );
		//fprintf( stdout, "round%d\n", loopcounter );
		// if ( loopcounter > 1000 ) { break; }
		fprintf( stdout, "token word is %s\n", token );
		// type_of_token = token_type( token );
		// fprintf( stdout, "[%s\t]\t:%s\n", t_type, token );
		/*
		if ( type_of_token == KEYWORD ) {
			type_of_keyword= keyword( token );
			fprintf( stdout, "[KEYWORD]:%s\n", keyword_str[type_of_token] );
			fprintf( stdout, "\tcalling keyword function\n" );
		} else if ( type_of_token == SYMBOL ) {
			symbol( symbol_string );
			fprintf( stdout, "[SYMBOL]:%s\n", symbol_string );
			fprintf( stdout, "\tcalling symbol function\n" );
		} else if ( type_of_token == INT_CONST ) {
			int_num = int_val( token );
			fprintf( stdout, "[INT_CONST]:%d\n", int_num );
			fprintf( stdout, "\tcalling int_const function\n" );
		} else if ( type_of_token == STRING_CONST ) {
			string_val( string_const );
			fprintf( stdout, "[STRING_CONST]:%s\n", string_const );
			fprintf( stdout, "\tcalling string_const function\n" );
		} else if ( type_of_token == IDENTIFIER ) {
			identifier( identifier_string ); 
			fprintf( stdout, "[IDENTIFIER]:%s\n", identifier_string );
			fprintf( stdout, "\tcalling identifier function\n" );
		} else {
			fprintf( stdout, "No token, if reach here, this means error\n" );
		}*/
	}
}

bool has_more_tokens( FILE * filepointer ) {

	char c;
	char tmp_c;
	int i = 0;

	// 入力ストリームの最終文字列まで読み込む
	// macでは\r\nなので，条件分岐でヒットするようにしておく
	while ( ( c = fgetc( filepointer ) ) == ' ' || c == '\t' || c == '\n' || c == '/' || c == '\r' ) {
		// fprintf( stdout, "space, tab, newline line code\n" ); // do nothing, goto next round
		if ( c == '/' ) {
			c = fgetc( filepointer ); // 現在のストリームの位置を進める
			if ( c == '/' ) {
				// 1行コメント行の場合, 改行までストリームから読み出し
				while ( ( c = fgetc( filepointer ) ) != '\r' && c != '\n' ) {
					// fprintf( stdout, "one line comment out %c\n", c );
				}
			} else if ( c == '*' ) {
				// 複数行に跨るコメント行の場合, 最後の*/まで読み出し
				tmp_c = ' '; // empty char set 対策
				while ( !( ( c = fgetc( filepointer ) ) == '/' && tmp_c == '*' ) ) {
					tmp_c = c;
					i++;
				}
				// fprintf( stdout, "last line is [tmp_c]:%d and [c]:%d\n", tmp_c, c );
			} else {
				// /１個の場合は演算子なため入力ストリームに書き戻す
				ungetc( c, filepointer );
				return true;
			}
		} 
	} 
	// fprintf( stdout, "word is %c\n", c );
	ungetc( c, filepointer ); // 入力ストリームから読み取った値を再度入寮ストリームへ書き戻す

	// cの値がアルファベットか演算子の場合, トークンが存在するため
	// trueを返す
	if ( isalpha( c ) || ispunct( c ) || isalnum( c ) ) {
		// fprintf( stdout, "%c", c );
		return true;
	} else {
		fprintf( stdout, "not string, ignore this word\n" );
		return false;
	}
}

void advance( FILE * fp ) {

	char c;
	int number = 0;

	c = fgetc( fp );

	if ( isalnum( c ) ) {
		// 文字列の場合
		token[number] = c;
		while ( isalpha( ( c = fgetc( fp ) ) ) ) {
			number++;
			token[number] = c;
		}
		ungetc( c, fp ); 
		// ループを抜けた際の文字は区切り文字等なので，
		// 入力文字列をストリーム\に書き直す必要がある
		number++;
		token[number] = '\0';
		return;
	} else if ( isdigit( c ) ) {
		// intergerConst
		// 整数の場合
		token[number] = c;
		while ( isdigit( ( c = fgetc( fp ) ) ) ) {
			number++;
			token[number] = c;
		}
		ungetc( c, fp );
		number++;
		token[number] = '\0';
		return;
	} else if ( ispunct( c ) ) {
		// symbol文字列
		token[number] = c;
		number++;
		token[number] = '\0';
		return;
	}
}

int token_type( char current[256] ) {
	if ( is_keyword( current ) ) {
		fprintf( stdout, "keyword\n" );
		strcpy( t_type, "keyword" );
		return KEYWORD;
	} else if ( is_symbol( current ) ) {
		fprintf( stdout, "symbol\n" );
		strcpy( t_type, "symbol" );
		return SYMBOL;
	} else if ( is_integer_constant( current ) ) {
		fprintf( stdout, "int_const\n" );
		strcpy( t_type, "int_const" );
		return INT_CONST;
	} else if ( is_string_constant( current ) ) {
		fprintf( stdout, "string_const\n" );
		strcpy( t_type, "string_const" );
		return STRING_CONST;
	} else if ( is_identifier( current ) )  {
		fprintf( stdout, "identifier\n" );
		strcpy( t_type, "is_identifier" );
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
	
	while ( c_token[i] != '\0' && c_token[i] != '"' ) {
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
			if ( i >= 1 ) {
				token[i-1] = '\0';
			}
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
			val_string[i] = '\0';
			return false;
		}
	}
	return true;
}
