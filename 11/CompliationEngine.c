#include "define.h"

void printTab( FILE * ofp, int depth ) {
	if ( isstdout ) {
		while ( depth ) {
			fprintf( stdout, "  " );
			depth--;
		}
	} else {
		while ( depth ) {
			fprintf( ofp, "  " );
			depth--;
		}
	}
}

void printTokenAndTag( FILE * ofp, char * type, char * thistoken, int depth ) {
	printTab( ofp, depth );
	if ( isstdout ) {
		fprintf( stdout, "<%s> %s </%s>\n", type, thistoken, type );
	} else {
		fprintf( ofp, "<%s> %s </%s>\n", type, thistoken, type   );
	}
}

void printTokenAndTagStart( FILE * ofp, char * thistoken, int depth ){
	printTab( ofp, depth );
	if ( isstdout ) {
		fprintf( stdout, "<%s>\n", thistoken );
	} else {
		fprintf( ofp, "<%s>\n", thistoken );
	}
}

void printTokenAndTagEnd( FILE * ofp, char * thistoken, int depth ) {
	printTab( ofp, depth );
	if ( isstdout ) {
		fprintf( stdout, "</%s>\n", thistoken);
	} else {
		fprintf( ofp, "</%s>\n", thistoken );
	}
}

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
	int depth = 0;

	while ( has_more_tokens( ifp ) ) {
		// list_Print();
		constructor();
		compile_Class( ifp, ofp, depth );
		//advance( ifp );
		//fprintf( stdout, "%s\n", token );
	}
}

int compile_Class( FILE * ifp, FILE * ofp, int depth ) {
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;
	list_t * p;

	printTokenAndTagStart( ofp, "class", depth );
	
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );

		if ( debug ) {
			fprintf( stdout, "%s, %d\n", token,  type_of_token );
		}
		if ( strcmp( token, "class" ) == 0 && type_of_token == KEYWORD ) {
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else {
			fprintf( stdout, "[ERROR]: Program must starts \"class\", compile canceled\n" );
			return -1; 
		}
	} 

	// クラス名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			printTokenAndTag( ofp, t_type, token, sec_depth );
			if ( !( p = list_Find_Node( token ) ) ) {
				list_Add( token );
				p = list_Find_Node( token ); 
				list_Init_Subrot( p );
			}
		} else {
			fprintf( stdout, "[ERROR]: Class name must be identifier\n" );
			return -1;
		}
	}

	if ( !compile_Symbol( ifp, ofp, '{', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: After class name, { was expected\n" );
		return -1;
	}


	// クラス変数，関数及びメソッドをコンパイルする
	while ( has_more_tokens(ifp) ) {
		// 大きめのサブルーチンを作成してそちらに処理を
		// すべて書いた方が可読性が上がる気がする
		advance(ifp);
		type_of_token = token_type(token);
		if ( type_of_token == KEYWORD && ( strcmp( token, "field" ) == 0 || strcmp( token, "static" ) == 0 ) ) {
			// クラス変数をコンパイルする
			if ( strcmp( token, "static" ) == 0 ) {
				cnt_static += 1;
			} else if ( strcmp( token, "field" ) == 0 ) {
				cnt_field += 1;
			}
			ungets( ifp, strlen(token) );
			compile_Class_Var_Dec( ifp, ofp, sec_depth );
		} else if (	type_of_token == KEYWORD && ( strcmp( token, "constructor" ) == 0 || 
				strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) ) {
			// 上記と同様
			// 関数及びメソッドをコンパイルする
			ungets( ifp, strlen(token) );
			compile_Subroutine_Dec( ifp, ofp, p, sec_depth );
		} else {
			if ( debug ) {
				fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
			}
			ungets( ifp, strlen( token ) );
			break;
		}
	}

	// 上記ループを抜けた時には入力ストリームに文字は残っていない
	// 最後に入力ストリームから受け取った文字列を入力ストリームに書き戻す
	// ungets( ifp, strlen( token ) );
	if ( !compile_Symbol( ifp, ofp, '}', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: End of Class compile, } was expected\n" );
		return -1;
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTokenAndTagEnd( ofp, "class", depth );
	
	return 1;
}

// class_var_decをコンパイルする
int compile_Class_Var_Dec( FILE * ifp, FILE * ofp, int depth )  {

	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__ );
	}
	
	printTokenAndTagStart( ofp, "classVarDec", depth );

	// static or field
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "static" ) == 0 || strcmp( token, "field" ) == 0 ) ) {
			printTokenAndTag( ofp, t_type, token, sec_depth );
			strcpy( propof, token );
		} else {
			fprintf( stdout, "[ERROR]:Class variable decleration must be static or field\n" );
			ungets( ifp, strlen( token ) );
			return -1;
		}
	}

	// 型のコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) {
				strcpy( typeof, token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} 
		} else if ( type_of_token == IDENTIFIER ) {
			if ( list_Find_Node( token ) ) {
				strcpy( typeof, token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else { 
				fprintf( stdout, "[ERROR]:Undefined Class name %s\n", token );
				ungets( ifp, strlen( token ) );
				return -1;
			}
		} else {
			fprintf( stdout, "[ERROR]:Type is not defined\n" );
			return -1;
		}
	}

	// 変数名のコンパイルを続ける
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == IDENTIFIER ) {
				if ( strcmp( propof, "static" ) == 0 ) {
					my_define( 1, token, typeof, propof, cnt_static );
					cnt_static += 1;
				} else if ( strcmp( propof, "field" ) == 0 ) {
					my_define( 1, token, typeof, propof, cnt_field );
					cnt_field += 1;
				}
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else if ( type_of_token == SYMBOL ) {
				if ( token[0] == ','  ) {
					ungets( ifp, strlen( token ) );
					compile_Symbol( ifp, ofp, ',', sec_depth ); 
				} else if ( token[0] == ';' ) {
					ungets( ifp, strlen( token ) );
					compile_Symbol( ifp, ofp, ';', sec_depth  );
					break;
				} else {
					fprintf( stdout, "[ERROR]:Split word must be , or ;\n" );
					return -1;
				}
			} else {
				fprintf( stdout, "[ERROR]: Var name next token is ; or , \n" );
				ungets( ifp, strlen( token ) );
				return -1;
			} 
		}
	}

	printTokenAndTagEnd( ofp, "classVarDec", depth );
	return 1;
}

int compile_Subroutine_Dec( FILE * ifp, FILE * ofp, list_t * class_pos, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "subroutineDec", depth );
	// サブルーチンの最初のトークンはconstructor, function, methodのいづれかで始まる
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "constructor" ) == 0 || strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) {
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				fprintf( stdout, "[ERROR]: First token must be constructor, function or method\n" );
				return -1;
			}
		}
	}

	// サブルーチンの型はvoid, int, char, boolean 及びクラス名のいずれか
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( 	strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || 
				strcmp( token, "boolean" ) == 0 || strcmp( token, "void" ) == 0 ) {
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				fprintf( stdout, "[ERROR]: Primitive type are  int, char or boolean, type is %s\n", token );
				return -1;
			}
		} else if ( type_of_token == IDENTIFIER ) {
			if ( class_pos ) {
				// printTab( sec_depth );
				// fprintf( stdout, "<identifier> %s </identifier>\n", token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				fprintf( stdout, "[ERROR]: Undefined Class name, %s\n", token );
				return -1;
			}
		} else {
			fprintf( stdout, "[ERROR]: Subroutine type must be int, char, boolean or defined class name, %s\n", token );
			return -1;
		}
	}

	// サブルーチン名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			printTokenAndTag( ofp, t_type, token, sec_depth );
			list_Add_Subrot( class_pos, token );
		} else {
			fprintf( stdout, "[ERROR]: Function name must be identifier\n" );
			return -1;
		}
	}

	// サブルーチン名の後のシンボルは(が来る
	if ( !compile_Symbol( ifp, ofp, '(', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: After subroutime name must be (\n" );
		return -1;
	}

	// パラメータリストをコンパイル
	compile_ParameterList( ifp, ofp, sec_depth );

	if ( !compile_Symbol( ifp, ofp, ')', sec_depth ) ) {
		return -1;
	}

	printTokenAndTagStart( ofp, "subroutineBody", sec_depth );
	int thd_depth = sec_depth+1;
	// サブルーチン本体をコンパイル
	// はじめのトークンは{
	compile_Symbol( ifp, ofp, '{', thd_depth );

	while ( has_more_tokens(ifp) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "%s\n", token );
		}
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			ungets( ifp, strlen( token ) );
			compile_Var_Dec( ifp, ofp, thd_depth );
		} else {
			if ( debug ) {
				fprintf( stdout, "break while loop because no more any var\n" );
				fprintf( stdout, "input word is %s\n", token );
			}
			ungets( ifp, strlen( token ) );
			break;
		}
	}

	// statementsをコンパイル
	compile_Statements( ifp, ofp, thd_depth );
	compile_Symbol( ifp, ofp, '}', thd_depth );

	printTokenAndTagEnd( ofp, "subroutineBody", sec_depth );

	printTokenAndTagEnd( ofp, "subroutineDec", depth );
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	return 1;
}

int compile_Statements( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	// このブロックをコンパイルする時に一番低い
	// インデント深さを記録する
	int sec_depth = depth + 1;
	
	printTokenAndTagStart( ofp, "statements", depth );
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == KEYWORD ) {
				if ( strcmp( token, "let" ) == 0 ) {

					compile_Let_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "if" ) == 0 ) {

					compile_If_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "while" ) == 0 ) {

					compile_While_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "do" ) == 0 ) {

					compile_Do_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "return" ) == 0 ) {

					compile_Return_Statement( ifp, ofp, sec_depth );

				}
			} else {
				if ( debug ) {
					fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
				}
				ungets( ifp , strlen( token ) );
				if ( debug ) {
					fprintf( stdout, "[%s]:Finish\n", __func__ );
				}
				break;
			}		
		} else {
			ungets( ifp, strlen( token ) );
			if ( debug ) {
				fprintf( stdout, "[%s]:Finish\n", __func__ );
			}
			// printTab( depth ); 
			// fprintf( stdout, "</statements>\n" );
			break;
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	// printTab( depth ); 
	// fprintf( stdout, "</statements>\n" );
	printTokenAndTagEnd( ofp, "statements", depth );
	return 1;
}

int compile_Var_Dec( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "varDec", depth );

	// var 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			strcpy( propof, token );
			cnt_var += 1;
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else {
			return -1;
		}
	}

	// 型名のコンパイル 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) ) {
			// プリミティブ型の場合
			strcpy( typeof, token );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else 	if ( list_Find_Node( token ) ) {
			// プリミティブ型以外の場合
			strcpy( typeof, token );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else { 
			fprintf( stdout, "[ERROR]: Var type must be int, char, boolean or classname\n" );
			ungets( ifp, strlen( token ) );
			return 0;
		}
	}

	// 変数名のコンパイル 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "next value is %c\n", token[0] );
		}
		if ( type_of_token == IDENTIFIER ) {
			my_define( 0, token, typeof, propof, cnt_var );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else {
			return -1;
		}
	}

	while ( 1 ) {
		// 
		if ( compile_Symbol( ifp, ofp, ',', sec_depth ) ) {
			; // do nothing
		} else if ( compile_Symbol( ifp, ofp, ';', sec_depth ) ) {
			printTokenAndTagEnd( ofp, "varDec", depth );
			return -1;
		}

		if ( has_more_tokens( ifp ) ) {
			advance( ifp );

			type_of_token = token_type( token );
 			if ( type_of_token == IDENTIFIER ) {
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				fprintf( stdout, "[ERROR] next token must be identifier\n" );
				return -1;
			}
		} else {
			fprintf( stdout, "[ERROR] no more any tokens\n" );
			return -1;
		}
	}

	return 1;
}

void ungets( FILE * ifp, int length ) {
	for ( int i = length-1 ; i >= 0 ; i-- ) {
		ungetc( token[i], ifp );
	}
}

int compile_subroutine_name( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_keyword;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		depth++;
		
		type_of_keyword = token_type( token );
		if ( type_of_keyword == IDENTIFIER ) {
			// printTab( depth );
 			// fprintf( stdout, "<identifier> %s </identifier>\n", token );
			printTokenAndTag( ofp, t_type, token, depth );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Next token must be identifier, subroutine name\n" );
		}
	}

	ungets( ifp, strlen( token )  );
	return 0;
}

int compile_ParameterList( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth + 1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "parameterList", depth );

	// サブルーチンのパラメータリストのコンパイルを開始する
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );
			if ( type_of_token == KEYWORD ) {
				// プリミティブ型の場合
				if ( strcmp( token, "int" ) == 0 || strcmp( token, "char " ) == 0 || strcmp( token, "boolean" ) ) {
					strcpy( typeof, token );		
					cnt_arg += 1;
					printTokenAndTag( ofp, t_type, token, sec_depth );
				} else {
					ungets( ifp, strlen( token ) );
					break;
				}
			} else if ( type_of_token == IDENTIFIER ) {
				// プリミティブ型以外の型の場合
				strcpy( typeof, token );
				cnt_arg += 1;
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				ungets( ifp, strlen( token ) );
				break;
			}
		} else {
			break;
		}

		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == IDENTIFIER ) {
				// 引数名をコンパイルする
				my_define( 0, token, typeof, propof, cnt_arg );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			}
		} else {
			break;
		}
		if ( compile_Symbol( ifp, ofp, ',', sec_depth ) ) {
			// goto next parameter		
			// 他に引数がある場合, ループをブレイクしない
		} else if ( compile_Symbol( ifp, ofp, ';', sec_depth ) ) {
			break;
		}
	}
	printTokenAndTagEnd( ofp, "parameterList", depth );
	return 0;
}
/*
int compile_statements( FILE * ifp ) {
	
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		if ( strcmp( token, "do" ) == 0 ) { 
			compile_do( ifp );
		} else if ( strcmp( token, "let" ) == 0 ) {
			compile_let( ifp );
		} else if ( strcmp( token, "while" ) == 0 ) {
			compile_while( ifp );
		} else if ( strcmp( token, "return" ) == 0 ) {
			compile_return( ifp );
		} else if ( strcmp( token, "if" ) == 0 ) {
			compile_if( ifp );
		}
		return 1;
	}
	return 0;
}
*/

/*
int compile_do( FILE * ifp ) {

	int type_of_token;

	// subroutinename
	if ( has_more_token( ifp ) ) {
		advance( ifp );
		
		// next token is subroutinename, identifier 
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

}
*/

/*
int compile_subroutine( FILE * ifp ) {
	int type_of_token;

	// subroutinename 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

	// symbol
	compile_Symbol( ifp, '(' );

	compile_expressionlist( ifp );
}
*/

/*
int compile_expreassionlist( FILE * ifp ) {
	int type_of_token;


	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
		
			compile_expression( ifp );
		}
	}

}
*/

int compile_Let_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int flag = 0;
	char before;
	int sec_depth = depth+1;

	// printTab( depth );
	// fprintf( stdout, "<letStatement>\n" );
	printTokenAndTagStart( ofp, "letStatement", depth );
	// printTab( sec_depth );
	// fprintf( stdout, "<keyword> let </keyword>\n" );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	// 変数名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			// printTab( sec_depth );
			// fprintf( stdout, "<identifier> %s </identifier>\n", token );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		}
	}

	// TODO ; beforeへの値いれを行う
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
	}

	if ( token[0] == '=' ) {
		ungetc( token[0], ifp );
		compile_Symbol( ifp, ofp, '=', sec_depth );
		compile_Expression( ifp, ofp, sec_depth );
	} else if ( token[0] == '[' ) {
		ungetc( token[0], ifp );
		compile_Symbol( ifp, ofp, '[', sec_depth );
		compile_Expression( ifp, ofp, sec_depth );
		compile_Symbol( ifp, ofp, ']', sec_depth );

		compile_Symbol( ifp, ofp, '=', sec_depth );
		compile_Expression( ifp, ofp, sec_depth );
	}
	
	compile_Symbol( ifp, ofp, ';', sec_depth ); 
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	// printTab( depth );
	// fprintf( stdout, "</letStatement>\n" );
	printTokenAndTagEnd( ofp, "letStatement", depth );

	return 1;
}

void compile_If_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	// printTab( depth );
	// fprintf( stdout, "<ifStatements>\n" );	
	printTokenAndTagStart( ofp, "ifStatement", depth );
	// printTab( sec_depth );
	// fprintf( stdout, "<keyword> if </keyword>\n" );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	compile_Symbol( ifp, ofp, '(', sec_depth );

	compile_Expression( ifp, ofp, sec_depth );

	compile_Symbol( ifp, ofp, ')', sec_depth );

	compile_Symbol( ifp, ofp, '{', sec_depth );
	
	compile_Statements( ifp, ofp, sec_depth );

	compile_Symbol( ifp, ofp, '}', sec_depth );
	fprintf( stdout, "check\n" );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "else" ) == 0 ) {
				// printTab( sec_depth );
				// fprintf( stdout, "<keyword> %s </keyword>\n", token );
				printTokenAndTag( ofp, t_type, token, sec_depth );

				if ( compile_Symbol( ifp, ofp, '{', sec_depth ) ) {
					compile_Statements( ifp, ofp, sec_depth );
					compile_Symbol( ifp, ofp, '}', sec_depth );
				}
			} else {
				ungets( ifp, strlen( token ) );
			}
		} else {
			ungets( ifp, strlen( token ) );
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	// printTab( depth );
	// fprintf( stdout, "</ifStatements>\n" );	
	printTokenAndTagEnd( ofp, "ifStatement", depth );
}

void compile_While_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	// printTab( depth );
	// fprintf( stdout, "<whileStatement>\n" );
	printTokenAndTagStart( ofp, "whileStatement", depth );
	// printTab( sec_depth );
	// fprintf( stdout, "<keyword> while </keyword>\n" );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	compile_Symbol( ifp, ofp, '(', sec_depth );

	compile_Expression( ifp, ofp, sec_depth );

	compile_Symbol( ifp, ofp, ')', sec_depth );

	compile_Symbol( ifp, ofp, '{', sec_depth );
	
	compile_Statements( ifp, ofp, sec_depth );

	compile_Symbol( ifp, ofp, '}', sec_depth );

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	// printTab( depth );
	// fprintf( stdout, "</whileStatement>\n" );
	printTokenAndTagEnd( ofp, "whileStatement", depth );

}

void compile_Do_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	list_t * p = list_Find_Node( token );
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	// printTab( depth );
	// fprintf( stdout, "<doStatement>\n" );
	printTokenAndTagStart( ofp, "doStatement", depth );
	// printTab( sec_depth );
	// fprintf( stdout, "<keyword> do </keyword>\n" );
	printTokenAndTag( ofp, t_type, token, sec_depth );
 	/*
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
	  		fprintf( stdout, "\t\t<identifier> %s </identifier>\n", token );
		}
	}
	*/
	compile_Subroutine_Call( ifp, ofp, p, sec_depth );
	compile_Symbol( ifp, ofp, ';', sec_depth ); 

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	// printTab( depth );
	// fprintf( stdout, "</doStatement>\n" );
	printTokenAndTagEnd( ofp, "doStatement", depth );
}

void compile_Subroutine_Call( FILE * ifp, FILE * ofp, list_t * class_pos, int depth ) {
	int type_of_token;
	char class_name[256];
	subroutine_name_t * p;
	list_t * lp;
	strcpy( class_name, token );

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			// if ( ( lp = list_Find_Node( token ) ) ) { 
				if ( debug ) {
					fprintf( stdout, "call subroutine, this class is registered at function list\n" );
				}
				// printTab( depth );
				// fprintf( stdout, "<identifier> %s </identifier>\n", token );
				printTokenAndTag( ofp, t_type, token, depth );
			// }
		}
	}

	if ( compile_Symbol( ifp, ofp, '(', depth ) ) {
		compile_Expression_List( ifp, ofp, depth );
		compile_Symbol( ifp, ofp, ')', depth );
	} else if ( compile_Symbol( ifp, ofp, '.', depth ) ) {
		if ( has_more_tokens( ifp) ) {
			advance( ifp );
			type_of_token = token_type( token );

			// if ( ( p = list_Find_Node_Subrot( lp, token ) ) ) {
				if ( debug ) {
					fprintf( stdout, "call subroutine, this method is registered at method list\n" );
				}
				// printTab( depth );
				// fprintf( stdout, "<identifier> %s </identifier>\n", token );
				printTokenAndTag( ofp, t_type, token, depth );
			// }

			if ( compile_Symbol( ifp, ofp, '(', depth ) ) {
				compile_Expression_List( ifp, ofp, depth );
				compile_Symbol( ifp, ofp, ')', depth );
			} else {
				fprintf( stdout, "[ERROR]: Subroutine Parameter must start (, %s\n", token );
				return;
			}
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
}

int compile_Return_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;
	int flag = 0;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	// printTab( depth );
	// fprintf( stdout, "<returnStatement>\n" );
	printTokenAndTagStart( ofp, "returnStatement", depth );
	// printTab( sec_depth );
	// fprintf( stdout, "<keyword> return </keyword>\n" );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL && token[0] == ';' ) {
			ungets( ifp, strlen( token ) );
			compile_Symbol( ifp, ofp, ';', sec_depth );
		} else {
			ungets( ifp, strlen( token ) );
			compile_Expression( ifp, ofp, sec_depth );
			compile_Symbol( ifp, ofp, ';', sec_depth );
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	// printTab( depth );
	// fprintf( stdout, "</returnStatement>\n" );
	printTokenAndTagEnd( ofp, "returnStatement", depth );
	return 1;
}

void compile_Expression( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;
	int flag = 0;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}


	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( 	type_of_token == INT_CONST || type_of_token == STRING_CONST || strcmp( token, "true" ) == 0 || 
			strcmp( token, "false" ) == 0 || strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {

			flag = 1;
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			flag = 1;
		} else if ( type_of_token == SYMBOL ) {
			// fprintf( stdout, "this is symbol\n" );
			if ( token[0] == '(' || token[0] == '-' || token[0] == '~' ) {
				flag = 1;
			}
		}
	}


	if ( flag == 1 ) {

		ungets( ifp, strlen( token ) );
		// printTab( depth );
		// fprintf( stdout, "<expression>\n" );
		printTokenAndTagStart( ofp, "expression", depth );
		compile_Term( ifp, ofp, sec_depth );

		while ( 1 ) {
			if ( 	compile_Symbol( ifp, ofp, '+', sec_depth ) || compile_Symbol( ifp, ofp,  '-', sec_depth ) || compile_Symbol( ifp, ofp, '*', sec_depth ) || 
				compile_Symbol( ifp, ofp, '/', sec_depth ) || compile_Symbol( ifp, ofp, '&', sec_depth ) || compile_Symbol( ifp, ofp, '|', sec_depth ) || 
				compile_Symbol( ifp, ofp, '<', sec_depth ) || compile_Symbol( ifp, ofp, '>', sec_depth ) || compile_Symbol( ifp, ofp, '=', sec_depth ) ) {
				compile_Term( ifp, ofp, sec_depth );
			} else {
				break;
			}
		}

		if ( debug ) {
			fprintf( stdout, "[%s]:Finish\n", __func__ );
		}

		// printTab( depth );
		// fprintf( stdout, "</expression>\n" );
		printTokenAndTagEnd( ofp, "expression", depth );

	} else {
		return;
	}
}

void compile_Term( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;
	list_t * p;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	// printTab( depth );
	// fprintf( stdout, "<term>\n" );
	printTokenAndTagStart( ofp, "term", depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		// fprintf( stdout, "%s\n", token );
		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
		}
		// fprintf( stdout, "%s\n", t_type );
		if ( type_of_token == INT_CONST ) {
			// integerConst
			flag = 1;
			// printTab( sec_depth );
			// fprintf( stdout, "<integerConstant> %d </integerConstant>\n", atoi( token ) );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else if ( type_of_token == STRING_CONST ) {
			// stringConst
			// "はシンボルではないので, 下に書いた部分では引っかからない
			flag = 1;
			get_stringconst( ifp );
			// printTab( sec_depth );
			// fprintf( stdout, "<stringConstant>" );
			// fprintf( stdout, " %s ", token );
			// fprintf( stdout, "</stringConstant>\n" );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else if (	strcmp( token, "true" ) == 0 || strcmp( token, "false" ) == 0 || 
			   	strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {
			// keywordConst
			flag = 1;
			// printTab( sec_depth );
			// fprintf( stdout, "<keyword> %s </keyword>\n", token );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			if ( ( p = list_Find_Node( token ) ) ) {
				// サブルーチン呼び出し
			flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Subroutine_Call( ifp, ofp, p, sec_depth );
			} else {
			flag = 1;
				// printTab( sec_depth );
				// fprintf( stdout, "<identifier> %s </identifier>\n", token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
				if ( compile_Symbol( ifp, ofp, '[', sec_depth ) ) {
					compile_Expression( ifp, ofp, sec_depth );
					compile_Symbol( ifp, ofp, ']', sec_depth );
				}
			}
		} else if ( type_of_token == SYMBOL ) {
			// fprintf( stdout, "this is symbol\n" );
			if ( token[0] == '(' ) {
				// '(' expression ')'
				flag = 1;
				ungetc( token[0], ifp );
				compile_Symbol( ifp, ofp, '(', sec_depth );
				compile_Expression( ifp, ofp, sec_depth );
				compile_Symbol( ifp, ofp, ')', sec_depth );
			} else if ( token[0] == '-' ) {
				flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Symbol( ifp, ofp, '-', sec_depth );
				compile_Term( ifp, ofp, sec_depth );
			} else if ( token[0] == '~' ) {
				flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Symbol( ifp, ofp, '~', sec_depth );
				compile_Term( ifp, ofp, sec_depth );
			}
		}
	}
	if ( flag == 0 ) {
		// fprintf( stdout, "write back\n" );
		ungets( ifp, strlen( token) );
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	// printTab( depth );
	// fprintf( stdout, "</term>\n" );
	printTokenAndTagEnd( ofp, "term", depth );
}

char compile_Symbol( FILE * ifp, FILE * ofp, char sym, int depth ) {
	int type_of_token;
	int is_tmp = 0;
	char tmp_token[10];

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__ );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL && token[0] == sym ) {
			// printTab( depth );
			if ( token[0] == '<' ) {
				strcpy( tmp_token, "&lt;" );
				is_tmp = 1;
			} else if ( token[0] == '>' ) {
				strcpy( tmp_token, "&gt;" );
				is_tmp = 1;
			} else if ( token[0] == '&' ) {
				strcpy( tmp_token, "&amp;" );
				is_tmp = 1;
			}
			if ( is_tmp ) {
				printTokenAndTag( ofp, t_type, tmp_token, depth );
			} else {
				printTokenAndTag( ofp, t_type, token, depth );
			}
			if ( debug ) {
				fprintf( stdout, "[%s]:Finish\n", __func__ );
			}
			return token[0];
		} else {
			if ( debug ) {
				fprintf( stdout, "%c %d\n", token[0], token[0] );
			}
			ungets( ifp, strlen( token ) );
			return 0;
		}
	}
	return 0;
} 

int compile_Expression_List( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;
	int flag = 0;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "expressionList", depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( 	type_of_token == INT_CONST || type_of_token == STRING_CONST || strcmp( token, "true" ) == 0 || 
				strcmp( token, "false" ) == 0 || strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {

			flag = 1;
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			flag = 1;
		} else if ( type_of_token == SYMBOL ) {
			if ( token[0] == '-' || token[0] == '~' || token[0] == '(' ) {
				flag = 1;
			}
		}

		ungets( ifp, strlen( token ) );
		if ( flag == 1 ) {
			compile_Expression( ifp, ofp, sec_depth );
			while ( compile_Symbol( ifp, ofp, ',', sec_depth )  ) {
				compile_Expression( ifp, ofp, sec_depth );
			}
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTokenAndTagEnd( ofp, "expressionList", depth );

	return 0;
}
