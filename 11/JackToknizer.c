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


	while ( has_more_tokens( ifp ) ) {
		advance( ifp );
		fprintf( stdout, "token word is %s\n", token );
	}
}

bool has_more_tokens( FILE * filepointer ) {

	char c;
	char tmp_c;
	int i = 0;

	// 入力ストリームの最終文字列まで読み込む
	// macでは\r\nなので，条件分岐でヒットするようにしておく
	while ( ( c = fgetc( filepointer ) ) == ' ' || c == '\t' || 
		c == '\n' || c == '/' || c == '\r' ) {
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
				// 空白及び/をその順番で書き戻す
				// cには/以外の文字がセットされているので, 直性/を書き戻す
				ungetc( c, filepointer );
				ungetc( '/', filepointer );
				return true;
			}
		} 
	} 
	ungetc( c, filepointer ); // 入力ストリームから読み取った値を再度入寮ストリームへ書き戻す

	// cの値がアルファベットか演算子の場合, トークンが存在するため
	// trueを返す
	if ( isalpha( c ) || ispunct( c ) || isalnum( c ) ) {
		// fprintf( stdout, "%c", c );
		return true;
	} else {
		// fprintf( stdout, "not string, ignore this word\n" );
		return false;
	}
}

int advance( FILE * fp ) {

	char c;
	int number = 0;
	c = fgetc( fp );

	if ( isdigit( c ) ) {
		// 文字列の場合
		token[number] = c;
		while ( isdigit( ( c = fgetc( fp ) ) ) ) {
			number++;
			token[number] = c;
		}
		ungetc( c, fp ); 
		// ループを抜けた際の文字は区切り文字等なので，
		// 入力文字列をストリーム\に書き直す必要がある
		number++;
		token[number] = '\0';
		return 1;
	} else if ( isalnum( c ) ) {
		// intergerConst
		// 整数の場合
		token[number] = c;
		while ( isalnum( ( c = fgetc( fp ) ) ) ) {
			number++;
			token[number] = c;
		}
		ungetc( c, fp );
		number++;
		token[number] = '\0';
		return 1;
	} else if ( ispunct( c ) ) {
		// symbol文字列
		// fprintf( stdout, "symbol char is %c\n", c );
		token[number] = c;
		number++;
		token[number] = '\0';
		return 1;
	}
	return 0;
}

void get_stringconst( FILE * fp ) {
	// 文字列の"は出力に含めない
	char c;
	int number = 0;

	while ( 1 ) {
		c = fgetc( fp );
		token[number] = c;
		if ( c == '"' ) { 
			token[number] = '\0';
			break; 
		}
		number++;
	}
}

int token_type( char current[256] ) {
	if ( is_keyword( current ) ) {
		// fprintf( stdout, "this is keyword\n" );
		strcpy( t_type, "keyword" );
		return KEYWORD;
	} else if ( is_symbol( current ) ) {
		// fprintf( stdout, "this is symbol\n" );
		strcpy( t_type, "symbol" );
		return SYMBOL;
	} else if ( is_integer_constant( current ) ) {
		// fprintf( stdout, "this is int_const\n" );
		strcpy( t_type, "integerConstant" );
		return INT_CONST;
	} else if ( is_identifier( current ) )  {
		// fprintf( stdout, "this is identifier\n" );
		strcpy( t_type, "identifier" );
		return IDENTIFIER;
	} else if ( is_string_constant( current ) ) {
		// fprintf( stdout, "this is string_const\n" );
		strcpy( t_type, "stringConstant" );
		return STRING_CONST;
	}
	return -1;
}

bool is_keyword( char c_token[256] ) {

	// fprintf( stdout, "[%s] %s\n", __func__, token );
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

	// fprintf( stdout, "[%s] %s\n", __func__, token );
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

	// fprintf( stdout, "[%s] %s, %d, %d\n", __func__, token, atoi( c_token ), token[0] );
	if ( ( num = atoi( c_token ) ) || c_token[0] == '0' ) {
		//fprintf( stdout, "this is string val\n" );
		if ( num >= 0 || num <= 32767 ) {
			return true;
		}
	}
	return false;
}

bool is_string_constant( char c_token[256] ) {
	// fprintf( stdout, "[%s] %s\n", __func__, token );
	int length = strlen( c_token );
	int i = 0;
	int str_flag = 1;

	// fprintf( stdout, "this is jadgement function of string or not\n" );
	// fprintf( stdout, "%d, %s, \n", c_token[0], c_token );
	if ( c_token[i] == '"' ) {
		/*i++;
		str_flag = 0;
		while ( c_token[i] != '"' ) {
			fprintf( stdout, "%c", c_token[i] );
			i++;
		}
		fprintf( stdout, "\n" );
		c_token[i] = '\0';*/
		//fprintf( stdout, "This is string\n" );
		return true;
	}

	/*
	if ( str_flag == 1 ) {
		return false;
	}
	*/

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
	int id_flag = 0;

	// fprintf( stdout, "[%s] %s\n", __func__, token );
	if (  ( val_string[0] >= 48 && val_string[0] <= 57 ) || val_string[0] == '"'  ) {
		return false;
	}

	// fprintf( stdout, "[%s] %d, %d, %s, \n", __func__, token[0], '0', token );
	i++;
	while ( i <= length  ) {
		id_flag = 1;
		if ( isalpha( val_string[i] ) || val_string[i] == '_' || ( val_string[i] >= '0' && val_string[i] <= '9' ) ) {
			i++;
		} else {
			val_string[i] = '\0';
			break;
		}
	}

	if ( id_flag ) {
		return true;
	} else {
		return false;
	}
}
