#include "define.h"

void printTab( int depth ) {
	while ( depth ) {
		fprintf( stdout, "  " );
		depth--;
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
		compile_Class( ifp, depth );
		//advance( ifp );
		//fprintf( stdout, "%s\n", token );
	}
}

int compile_Class( FILE * ifp, int depth ) {
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;

	printTab( depth );
	fprintf( stdout, "<class>\n" );
	
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );

		if ( debug ) {
			fprintf( stdout, "%s, %d\n", token,  type_of_token );
		}
		if ( strcmp( token, "class" ) == 0 && type_of_token == KEYWORD ) {
			printTab( sec_depth );
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
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
			printTab( sec_depth );
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			list_Add( token );
		} else {
			fprintf( stdout, "[ERROR]: Class name must be identifier\n" );
			return -1;
		}
	}

	if ( !compile_Symbol( ifp, '{', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: After class name, { was expected\n" );
		return -1;
	}


	while ( has_more_tokens(ifp) ) {
		// 大きめおサブルーチンを作成してそちらに処理を
		// すべて書いた方が可読性が上がる気がする
		//fprintf( stdout, "<classVarDec>\n" );
		advance(ifp);
		type_of_token = token_type(token);
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			ungets( ifp, strlen(token) );
			compile_Class_Var_Dec( ifp, sec_depth );
		} else if (	type_of_token == KEYWORD && ( strcmp( token, "constructor" ) == 0 || 
				strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) ) {
			// 上記と同様
			ungets( ifp, strlen(token) );

			compile_Subroutine_Dec( ifp, sec_depth );
		} else {
			if ( debug ) {
				fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
			}
			ungets( ifp, strlen( token ) );
			break;
		}
	}

	// 上記ループを抜けた場合入力ストリームに文字は残っていない
	// 最後に入力ストリームから受け取った文字列を入力ストリームに書き戻す
	// ungets( ifp, strlen( token ) );
	if ( !compile_Symbol( ifp, '}', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: End of Class compile, } was expected\n" );
		return -1;
	}
	//printTab( depth );
	//fprintf( stdout, "<keyword> %s </keyword>\n", token );
	

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTab( depth );
	fprintf( stdout, "</class>\n" );
	
	return 1;
}
// class_var_decをコンパイルする
int compile_Class_Var_Dec( FILE * ifp, int depth )  {

	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__ );
	}
	
	printTab( depth );
	fprintf( stdout, "<varDec>\n" );

	// static or field
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		// fprintf( stdout, "===>%s\n", token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "static" ) == 0 || strcmp( token, "field" ) == 0 ) ) {
			printTab( sec_depth );
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		} else {
			ungets( ifp, strlen( token ) );
			return 0;
		}
	}

	// 型のコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) {
				printTab( sec_depth );
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			} 
		} else if ( type_of_token == IDENTIFIER ) {
			if ( list_Find_Node( token ) ) {
				printTab( sec_depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			} else { 
				fprintf( stdout, "[ERROR]: Var type must be int, char or boolean\n" );
				ungets( ifp, strlen( token ) );
				return 0;
			}
		}
	}

	// 区切り文字が, である限り変数名のコンパイルを続ける
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			// 変数名をコンパイル
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == IDENTIFIER ) {
				printTab( sec_depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			} else if ( type_of_token == SYMBOL ) {
				if ( compile_Symbol( ifp, ',', sec_depth ) ) {
					; // do nothing
				} else if ( compile_Symbol( ifp, ';', sec_depth ) ) {
					break;
				}
			} else {
				fprintf( stdout, "[ERROR]: Var name next token is ; or , \n" );
				ungets( ifp, strlen( token ) );
				return 0;
			} 
		}
	}

	printTab( depth );
	fprintf( stdout, "</varDec>\n" );
	return 1;
}

int compile_Subroutine_Dec( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	printTab( depth );
	fprintf( stdout, "<subroutineDec>\n" );
	// サブルーチンの最初のトークンはconstructor, function, methodのいづれかで始まる
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "constructor" ) == 0 || strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) {
				// printTab( depth );
				// fprintf( stdout, "<subroutineDec>\n" );
				printTab( sec_depth );
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
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
				printTab( sec_depth );
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			} else {
				fprintf( stdout, "[ERROR]: Function type must be int, char or boolean\n" );
				return -1;
			}
		}
	}

	// サブルーチン名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			printTab( sec_depth );
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Function name must be identifier\n" );
			return -1;
		}
	}

	// サブルーチン名の後のシンボルは(が来る
	if ( !compile_Symbol( ifp, '(', sec_depth ) ) {
		fprintf( stdout, "[ERROR]: After subroutime name must be (\n" );
		return -1;
	}

	// パラメータリストをコンパイル
	compile_ParameterList( ifp, sec_depth );

	if ( !compile_Symbol( ifp, ')', sec_depth ) ) {
		return -1;
	}

	
	printTab( sec_depth );
	fprintf( stdout, "<subroutineBody>\n" );
	int thd_depth = sec_depth+1;
	// サブルーチン本体をコンパイル
	// はじめのトークンは{
	compile_Symbol( ifp, '{', thd_depth );

	while ( has_more_tokens(ifp) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "%s\n", token );
		}
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			ungets( ifp, strlen( token ) );
			compile_Var_Dec( ifp, thd_depth );
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
	compile_Statements( ifp, thd_depth );
	compile_Symbol( ifp, '}', thd_depth );

	printTab( sec_depth );
	fprintf( stdout, "</subroutineBody>\n" );

	printTab( depth );
	fprintf( stdout, "</subroutineDec>\n" );
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	return 1;
}

int compile_Statements( FILE * ifp, int depth ) {
	int type_of_token;
	// このブロックをコンパイルする時に一番低い
	// インデント深さを記録する
	int sec_depth = depth + 1;
	//depth++;
	printTab( depth );
	fprintf( stdout, "<statements>\n" );

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == KEYWORD ) {
				if ( strcmp( token, "let" ) == 0 ) {

					compile_Let_Statement( ifp, sec_depth );

				} else if ( strcmp( token, "if" ) == 0 ) {

					compile_If_Statement( ifp, sec_depth );

				} else if ( strcmp( token, "while" ) == 0 ) {

					compile_While_Statement( ifp, sec_depth );

				} else if ( strcmp( token, "do" ) == 0 ) {

					compile_Do_Statement( ifp, sec_depth );

				} else if ( strcmp( token, "return" ) == 0 ) {

					compile_Return_Statement( ifp, sec_depth );

				}
			} else {
				if ( debug ) {
					fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
				}
				ungets( ifp , strlen( token ) );
				if ( debug ) {
					fprintf( stdout, "[%s]:Finish\n", __func__ );
				}
				return -1;
			}		
		} else {
			ungets( ifp, strlen( token ) );
			if ( debug ) {
				fprintf( stdout, "[%s]:Finish\n", __func__ );
			}
			return -1;
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	printTab( depth ); 
	fprintf( stdout, "</statements>\n" );
	return 1;
}

int compile_Var_Dec( FILE * ifp, int depth ) {
	/* var type varname(identifier) (, varname)* ;*/
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<varDec>\n" );

	// var 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			printTab( sec_depth );
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		} else {
			return -1;
		}
	}

	// type 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) ) {
			printTab( sec_depth );
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		} else 	if ( list_Find_Node( token ) ) {
			printTab( sec_depth );
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		} else { 
			fprintf( stdout, "[ERROR]: Var type must be int, char, boolean or classname\n" );
			ungets( ifp, strlen( token ) );
			return 0;
		}
	}

	// varname 
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "next value is %c\n", token[0] );
		}
		if ( type_of_token == IDENTIFIER ) {
			printTab( sec_depth );
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		} else {
			return -1;
		}
	}

	while ( 1 ) {
		// 
		if ( compile_Symbol( ifp, ',', sec_depth ) ) {
			; // do nothing
		} else if ( compile_Symbol( ifp, ';', sec_depth ) ) {
			return -1;
		}

		if ( has_more_tokens( ifp ) ) {
			advance( ifp );

			type_of_token = token_type( token );
 			if ( type_of_token == IDENTIFIER ) {
				printTab( sec_depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			} else {
				fprintf( stdout, "[ERROR] next token must be identifier\n" );
				return -1;
			}
		} else {
			fprintf( stdout, "[ERROR] no more any tokens\n" );
			return -1;
		}
	}
	printTab( depth );
	fprintf( stdout, "</varDec>\n" );

	return 1;
}

void ungets( FILE * ifp, int length ) {
	for ( int i = length-1 ; i >= 0 ; i-- ) {
		ungetc( token[i], ifp );
	}
}

int compile_subroutine_name( FILE * ifp, int depth ) {
	int type_of_keyword;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		depth++;
		
		type_of_keyword = token_type( token );
		if ( type_of_keyword == IDENTIFIER ) {
			printTab( depth );
 			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Next token must be identifier, subroutine name\n" );
		}
	}

	ungets( ifp, strlen( token )  );
	return 0;
}

int compile_ParameterList( FILE * ifp, int depth ) {
	int type_of_token;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<parameterList>\n" );

	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );
			if ( type_of_token == KEYWORD ) {
				if ( strcmp( token, "int" ) == 0 || strcmp( token, "char " ) == 0 || strcmp( token, "boolean" ) ) {
					printTab( depth );
					fprintf( stdout, "<keyword> %s </keyword>\n", token );
				} else {
					ungets( ifp, strlen( token ) );
					return -1;
				}
			} else if ( type_of_token == IDENTIFIER ) {
				printTab( depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			} else {
				ungets( ifp, strlen( token ) );
				return -1;
			}
		} else {
			return -1;
		}

		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == IDENTIFIER ) {
				printTab( depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			}
		} else {
			return -1;
		}
		if ( compile_Symbol( ifp, ',', depth ) ) {
			// goto next parameter		
		} else if ( compile_Symbol( ifp, ';', depth ) ) {
			break;
		}
	}
	printTab( depth );
	fprintf( stdout, "</parameterList>\n" );
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

int compile_Let_Statement( FILE * ifp, int depth ) {
	int type_of_token;
	int flag = 0;
	char before;
	int sec_depth = depth+1;

	printTab( depth );
	fprintf( stdout, "<letStatement>\n" );
	printTab( sec_depth );
	fprintf( stdout, "<keyword> let <keyword>\n" );

	// 変数名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			printTab( sec_depth );
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

	// TODO ; beforeへの値いれを行う
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
	}

	if ( token[0] == '=' ) {
		ungetc( token[0], ifp );
		compile_Symbol( ifp, '=', sec_depth );
		compile_Expression( ifp, sec_depth );
	} else if ( token[0] == '[' ) {
		ungetc( token[0], ifp );
		compile_Symbol( ifp, '[', sec_depth );
		compile_Expression( ifp, sec_depth );
		compile_Symbol( ifp, ']', sec_depth );

		compile_Symbol( ifp, '=', sec_depth );
		compile_Expression( ifp, sec_depth );
	}
	
	compile_Symbol( ifp, ';', sec_depth ); 
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	printTab( depth );
	fprintf( stdout, "</letStatement>\n" );

	return 1;
}

void compile_If_Statement( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<ifStatements>\n" );	
	printTab( sec_depth );
	fprintf( stdout, "<keyword> if </keyword>\n" );

	compile_Symbol( ifp, '(', sec_depth );

	compile_Expression( ifp, sec_depth );

	compile_Symbol( ifp, ')', sec_depth );

	compile_Symbol( ifp, '{', sec_depth );
	
	compile_Statements( ifp, sec_depth );

	compile_Symbol( ifp, '}', sec_depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "else" ) ) {
				printTab( sec_depth );
				fprintf( stdout, "<keyword> %s </keyword>\n", token );

				if ( compile_Symbol( ifp, '{', sec_depth ) ) {
					compile_Statements( ifp, sec_depth );
					compile_Symbol( ifp, '}', sec_depth );
				}
			}
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	printTab( depth );
	fprintf( stdout, "</ifStatements>\n" );	
}

void compile_While_Statement( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	printTab( depth );
	fprintf( stdout, "<whileStatements>\n" );
	printTab( sec_depth );
	fprintf( stdout, "<keyword> while </keyword>\n" );

	compile_Symbol( ifp, '(', sec_depth );

	compile_Expression( ifp, sec_depth );

	compile_Symbol( ifp, ')', sec_depth );

	compile_Symbol( ifp, '{', sec_depth );
	
	compile_Statements( ifp, sec_depth );

	compile_Symbol( ifp, '}', sec_depth );

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	printTab( depth );
	fprintf( stdout, "<whileStatements>\n" );


}

void compile_Do_Statement( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	list_t * p = list_Find_Node( token );
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<doStatements>\n" );
	printTab( sec_depth );
	fprintf( stdout, "<keyword> do <keyword>\n" );
 	/*
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
	  		fprintf( stdout, "\t\t<identifier> %s </identifier>\n", token );
		}
	}
	*/
	compile_Subroutine_Call( ifp, p, sec_depth );
	compile_Symbol( ifp, ';', sec_depth ); 

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTab( depth );
	fprintf( stdout, "</doStatements>\n" );
}

void compile_Subroutine_Call( FILE * ifp, list_t * class_pos, int depth ) {
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
			if ( lp = list_Find_Node( token ) ) { 
				if ( debug ) {
					fprintf( stdout, "call subroutine, this class is registered at function list\n" );
				}
				printTab( depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			}
		}
	}

	if ( compile_Symbol( ifp, '(', depth ) ) {
		compile_Expression_List( ifp, depth );
		compile_Symbol( ifp, ')', depth );
	} else if ( compile_Symbol( ifp, '.', depth ) ) {
		if ( has_more_tokens( ifp) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( p = list_Find_Node_Subrot( lp, token ) ) {
				if ( debug ) {
					fprintf( stdout, "call subroutine, this method is registered at method list\n" );
				}
				printTab( depth );
				fprintf( stdout, "<identifier> %s <identifier>\n", token );
			}

			if ( compile_Symbol( ifp, '(', depth ) ) {
				compile_Expression_List( ifp, depth );
				compile_Symbol( ifp, ')', depth );
			}
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
}

void compile_Return_Statement( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<returnStatements>\n" );
	printTab( sec_depth );
	fprintf( stdout, "<keyword> return <keyword>\n" );

	if ( compile_Symbol( ifp, ';', sec_depth ) ) {
		compile_Expression( ifp, sec_depth );
	} else {
		compile_Expression( ifp, sec_depth );
		compile_Symbol( ifp, ';', sec_depth );
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTab( depth );
	fprintf( stdout, "</returnStatements>\n" );
}

void compile_Expression( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	printTab( depth );
	fprintf( stdout, "<expression>\n" );
	compile_Term( ifp, sec_depth );

	while ( 1 ) {
		if ( 	compile_Symbol( ifp, '+', sec_depth ) || compile_Symbol( ifp, '-', sec_depth ) || compile_Symbol( ifp, '*', sec_depth ) || 
			compile_Symbol( ifp, '/', sec_depth ) || compile_Symbol( ifp, '&', sec_depth ) || compile_Symbol( ifp, '|', sec_depth ) || 
			compile_Symbol( ifp, '<', sec_depth ) || compile_Symbol( ifp, '>', sec_depth ) || compile_Symbol( ifp, '=', sec_depth ) ) {
			compile_Term( ifp, sec_depth );
		} else {
			break;
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	
	printTab( depth );
	fprintf( stdout, "</expression>\n" );
}

void compile_Term( FILE * ifp, int depth ) {
	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;
	list_t * p;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<term>\n" );

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
			printTab( sec_depth );
			fprintf( stdout, "<integerConstant> %d </integerConstant>\n", atoi( token ) );
		} else if ( type_of_token == STRING_CONST ) {
			// stringConst
			// "はシンボルではないので, 下に書いた部分では引っかからない
			flag = 1;
			get_stringconst( ifp );
			printTab( sec_depth );
			fprintf( stdout, "<stringConstant>" );
			fprintf( stdout, " %s ", token );
			printTab( sec_depth );
			fprintf( stdout, "</stringConstant>\n" );
		} else if (	strcmp( token, "true" ) == 0 || strcmp( token, "false" ) == 0 || 
			   	strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {
			// keywordConst
			flag = 1;
			printTab( sec_depth );
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			if ( p = list_Find_Node( token ) ) {
				// サブルーチン呼び出し
			flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Subroutine_Call( ifp, p, sec_depth );
			} else {
			flag = 1;
				printTab( sec_depth );
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
				if ( compile_Symbol( ifp, '[', sec_depth ) ) {
					compile_Expression( ifp, sec_depth );
					compile_Symbol( ifp, ']', sec_depth );
				}
			}
		} else if ( type_of_token == SYMBOL ) {
			// fprintf( stdout, "this is symbol\n" );
			if ( token[0] == '(' ) {
				// '(' expression ')'
			flag = 1;
				ungetc( token[0], ifp );
				compile_Symbol( ifp, '(', sec_depth );
				compile_Expression( ifp, sec_depth );
				compile_Symbol( ifp, ')', sec_depth );
			} else if ( token[0] == '-' || token[0] == '~' ) {
			flag = 1;
				if ( !compile_Symbol( ifp, '-', sec_depth ) ) {
					compile_Symbol( ifp, '~', sec_depth );
				}
				compile_Term( ifp, sec_depth );
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

	printTab( depth );
	fprintf( stdout, "</term>\n" );
}

char compile_Symbol( FILE * ifp, char sym, int depth ) {
	int type_of_token;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__ );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL && token[0] == sym ) {
			printTab( depth );
			fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
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

int compile_Expression_List( FILE * ifp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTab( depth );
	fprintf( stdout, "<expressionList>\n" );

	if ( has_more_tokens( ifp ) ) {
		compile_Expression( ifp, sec_depth );
		while ( compile_Symbol( ifp, ',', sec_depth )  ) {
			compile_Expression( ifp, sec_depth );
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTab( depth );
	fprintf( stdout, "</expressionList>\n" );

	return 0;
}
