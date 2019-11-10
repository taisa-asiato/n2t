#include "define.h"

void compile_main( FILE * ifp, FILE * ofp ) {
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
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			type_of_keyword = keyword( token );

			switch ( type_of_token ) {
				case CALSS: 
					compile_Class();

				case METHOD:

			}
			fprintf( stdout, "[KEYWORD]:%s\n", token );
			fprintf( stdout, "\tcalling keyword function\n" );
		} else if ( type_of_token == SYMBOL ) {
			symbol( symbol_string );
			fprintf( stdout, "[SYMBOL]:%s\n", token );
			fprintf( stdout, "\tcalling symbol function\n" );
		} else if ( type_of_token == INT_CONST ) {
			int_num = int_val( token );
			fprintf( stdout, "[INT_CONST]:%d\n", atoi( token ) );
			fprintf( stdout, "\tcalling int_const function\n" );
		} else if ( type_of_token == STRING_CONST ) {
			string_val( string_const );
			fprintf( stdout, "[STRING_CONST]:%s\n", token );
			fprintf( stdout, "\tcalling string_const function\n" );
		} else if ( type_of_token == IDENTIFIER ) {
			identifier( identifier_string ); 
			fprintf( stdout, "[IDENTIFIER]:%s\n", token );
			fprintf( stdout, "\tcalling identifier function\n" );
		}
	}
}

int compile_Class( FILE * ifp ) {
	fprintf( stdout, "<class>\n" );

	if ( has_more_tokens( ifp ) ) {
		advance();
		if ( strcmp( token, "Class" ) == 0 ) {
			fprintf( stdout, "\t<keyword> %s </keyword>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Program must starts \"Class\", compile canceled\n" );
			return;
		}


		// 次のトークンはクラス名を表す文字列( identifier )
		compile_class_name( ifp );

		// クラス名の次はシンボル"{"が来る
		compile_symbol( ifp, '{' );


		// 大きめおサブルーチンを作成してそちらに処理を
		// すべて書いた方が可読性が上がる気がする
		fprintf( stdout, "<classVarDec>\n" );
		compile_class_ver_dec();
		fprintf( stdout, "</subroutineDec>\n" );


		// 上記と同様
		fprintf( stdout, "<subroutineDec>\n" );
		compile_subroutine_var_dec();
		fprintf( stdout, "</subroutineDec>\n" );

		compile_symbol( '}' );
	}
}

int compile_class_name( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advane();
		type_of_token = keyword();
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Class Name must be identifier\n" );
		}
	} else {
		fprintf( stdout, "[ERROR]: No any input, %s\n", __func__ );
	}

	return 0;
}

int compile_symbol( FILE * ifp, char symbol_char ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = keyword();
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == symbol_char ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
				return 1;
			} else {
				fprintf( stdout, "[ERROR]: Syntax error, %c is expected\n", symbol_char );
			}
		} 
	} else {
		fprintf( stdout, "[ERROR]: No any input, %s\n", __func__ );
	}

	return 0;
}

int compile_class_var_dec( ifp )  {

	while ( has_more_tokens( ifp ) ) {
		advance();
		if ( strcmp( token, "static" ) == 0 || strcmp( token, "field" ) == 0 ) {
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
			compile_var_type( ifp );

		} else {
			for ( int i = 0 ; token[i] != '\0' ; i++ ) {
				unfgetc( token[i], ifp );
			}
			break;
		}
	}
}

int compile_var_type( FILE * ifp ) {
	int token_of_number;
	int flag = 0;

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		if ( token_of_number = token_type( token ) == KEYWORD ) {
			if ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 || strcmp( token, "void") == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n" );
				compile_var_name( ifp );
				compile_symbol( ';' );
				return 1;
			} else {
				fprintf( stdout, "[ERROR]: Next token is must be type( int, char, boolean, void), %s\n", __func__ );
			}
		} else {
			fprintf( stdout, "[ERROR]: Next token is must be keyword( type ), %s\n", __func__  );
		}
	}

	// typeでなかった場合はトークンを入力ストリームに書き戻す
	for ( int i = 0 ; token[i] != '\0' ; i++ ) {
		unfgetc( token[i], ifp );
	}

	return 0;
}	

void compile_subroutine_dec() {
	int type_of_keyword;

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		
		token_of_number = token_type( token );
		if ( token_of_number == KEYWORD ) {
			if ( strcmp( token, "constructor" ) == 0 || strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) { 
				fprintf( stdout, "<keyword> %s </keyword>\n" );
				
			}  else {
				fprintf( stdout, "[ERROR]: Next token is keyword( constructor, function, mehtod) is expected\n" );
			}
		} 
	} 
}	
/*
void type( char * cp ) {
	
	if ( 
		strcmp( cp, "int" ) == 0 || strcmp( cp, "char" ) == 0 || 
		strcmp( cp, "boolean" ) == 0 || cp == identifier ) {
		
	}
}
*/

int compile_var_name( FILE * ifp ) {
	int type_of_keyword;

	if( has_more_tokens( ifp ) ) {
		advance( ifp );
		
		type_of_keyword = token_type( token );
		if( type_of_keyword == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Var name is identifier\n" );
		}
	}
	return 0;
}
