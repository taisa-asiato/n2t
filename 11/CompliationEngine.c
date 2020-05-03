#include "define.h"

void printTab( FILE * ofp, int depth ) {
	if ( isstdout & xml ) {
		while ( depth ) {
			fprintf( stdout, "  " );
			depth--;
		}
	} else {
		while ( depth ) {
			//fprintf( ofp, "  " );
			depth--;
		}
	}
}

void printTokenAndTag( FILE * ofp, char * type, char * thistoken, int depth ) {
	printTab( ofp, depth );
	if ( isstdout & xml ) {
		fprintf( stdout, "<%s> %s </%s>\n", type, thistoken, type );
	} else {
		//fprintf( ofp, "<%s> %s </%s>\n", type, thistoken, type   );
	}
}

void printTokenAndTagStart( FILE * ofp, char * thistoken, int depth ){
	printTab( ofp, depth );
	if ( isstdout &xml ) {
		fprintf( stdout, "<%s>\n", thistoken );
	} else {
		//fprintf( ofp, "<%s>\n", thistoken );
	}
}

void printTokenAndTagEnd( FILE * ofp, char * thistoken, int depth ) {
	printTab( ofp, depth );
	if ( isstdout & xml ) {
		fprintf( stdout, "</%s>\n", thistoken);
	} else {
		//fprintf( ofp, "</%s>\n", thistoken );
	}
}

void printTokenStatus( FILE * ofp, char * thistoken, int depth ) {
	if ( debug ) {
		fprintf(stdout, "[%s]\n", __func__);
	}

	scope_t * tmp = list_Find_Scope( thistoken );
	int local_kind = kind_Of( thistoken );
	int local_type = type_Of( thistoken );
	int local_index = index_Of( thistoken );
	int isused = Is_Used( thistoken );
	int isdefined = Is_Defined( thistoken );

	if (debug) {
		fprintf(stdout, "printout token status\n");
	}

	if ( isstdout & xml ) {
		if ( tmp ) {
			printTab( ofp, depth );
			fprintf( stdout, "<name> %s </name>\n", thistoken );

			printTab( ofp, depth );
			fprintf( stdout, "<property> %s </property>\n", tmp->proper );

			printTab( ofp, depth );
			fprintf( stdout, "<type> %s </type\n", tmp->type );

			printTab( ofp, depth );
			fprintf( stdout, "<index> %d </index>\n", local_index );
			if ( strcmp( propof, "var" ) == 0 ) {
				printTab( ofp, depth );
				fprintf( stdout, "<status> Defined </status>\n" );
			} else {
				printTab( ofp, depth );
				fprintf( stdout, "<status> Used </status>\n" );
			}
		} else {
			printTab( ofp, depth );
			fprintf( stdout, "no registerd symbol : %s\n", thistoken );
		}
	} else if ( isstdout & xml ) {
		fprintf( stdout, "push " );
	} else {
		//fprintf( ofp, "<status> %s %s %s %d </status>\n",thistoken, propof, my_typeof, local_index );
	}
}

void printSubrotStatus( FILE * ofp, list_t * class_p, char * thistoken, int depth ) {
	char classdotfunc[256];

	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__ );
	}

	if ( isstdout & xml ) {
		printTab( ofp, depth );
		fprintf( stdout, "<subrot_name> %s </subrot_name>\n", thistoken );

		printTab( ofp, depth );
		fprintf( stdout, "<belong_class> %s </belong_class>\n", class_p->symbol_name );
	} else if ( isstdout & debug ) {
		fprintf( stdout, "[Debugging]:function %s.%s\n", class_p->symbol_name, thistoken );
	} else {
		//fprintf( ofp, "function %s.%s\n", class_p->symbol_name, thistoken );
	}
}

void printClassStatus( FILE * ofp, list_t * clsp, char * thistoken, int depth ) {
	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__ );
	}

	if ( isstdout & xml ) {
		printTab( ofp, depth );
		fprintf( stdout, "<this_class> %s </this_class>\n", clsp->symbol_name );
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
	if_index = 0;
	while_index = 0;

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
			if ( !( p = list_Find_Node( token ) ) ) {
				list_Add( token );
				p = list_Find_Node( token ); 
				list_Init_Subrot( p );
			}
			printTokenAndTag( ofp, t_type, token, sec_depth );
			strcpy( thisclassname, token );
			printClassStatus( ofp, p, token, depth );
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
			
			ungets( ifp, strlen(token) );
			compile_Class_Var_Dec( ifp, ofp, sec_depth );
		} else if (	type_of_token == KEYWORD && ( strcmp( token, "constructor" ) == 0 || 
					strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) ) {
			// 上記と同様
			// 関数及びメソッドをコンパイルする
			ungets( ifp, strlen(token) );
			init_SubroutineTable();
			cnt_var = 0;
			cnt_arg = 0;
			compile_Subroutine_Dec( ifp, ofp, p, sec_depth );
			if ( debug ) {
				fprintf( stdout, "[%s]:internal %s\n", __func__, token );
			}
			delete_SubroutineTable();
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
			symbol_define = 1;
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
				strcpy( my_typeof, token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} 
		} else if ( type_of_token == IDENTIFIER ) {
			// ユーザ宣言の型/クラスの場合
 			list_Print();
			if ( list_Find_Node( token ) ) {
				strcpy( my_typeof, token );
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

	// 変数名のコンパイル
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( type_of_token == IDENTIFIER ) {
				if ( strcmp( propof, "static" ) == 0 ) {
					my_define( 1, token, my_typeof, propof, cnt_static );
					cnt_static += 1;
				} else if ( strcmp( propof, "field" ) == 0 ) {
					my_define( 1, token, my_typeof, propof, cnt_field );
					cnt_field += 1;
				}
				//printTokenAndTag( ofp, t_type, token, sec_depth );
				printTokenStatus( ofp, token, depth );
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
	symbol_define = 0;
	return 1;
}

int compile_Subroutine_Dec( FILE * ifp, FILE * ofp, list_t * class_pos, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;
	int func_type = 0; 
	char function_name[256];
	char function_type[256];
	//int if_index = 0;
	//int while_index = 0;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "subroutineDec", depth );
	// サブルーチンの最初のトークンはconstructor, function, methodのいづれかで始まる
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( 	strcmp( token, "constructor" ) == 0 
				|| strcmp( token, "function" ) == 0 
				|| strcmp( token, "method" ) == 0 ) {
				if ( strcmp( token, "constructor" ) == 0 ) {
					func_type = CONSTRUCTOR;
				} else if ( strcmp( token, "function" ) == 0 ) {
					func_type = FUNCTION;
				} else if ( strcmp( token, "method" ) == 0 ) {
					func_type = METHOD;
				}
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
			if ( 	strcmp( token, "int" ) == 0 
				|| strcmp( token, "char" ) == 0 
				|| strcmp( token, "boolean" ) == 0 
				|| strcmp( token, "void" ) == 0 ) {
				strcpy( function_type, token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
			} else {
				fprintf( stdout, "[ERROR]: Primitive type are  int, char or boolean, type is %s\n", token );
				return -1;
			}
		} else if ( type_of_token == IDENTIFIER ) {
			if ( class_pos ) {
				// サブルーチンの型なのでサブルーチンの詳細情報は下記で出力する
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
			printSubrotStatus( ofp, class_pos, token, sec_depth );
			strcpy( function_name, token );
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
	if ( debug ) {
		fprintf( stdout, "[%s]:after parameter list\n", __func__ );
	}


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

	char classdotfunc[256];
	sprintf( classdotfunc, "%s.%s", class_pos->symbol_name, function_name );
	print_All_Function_Symbol( list_Find_Scope_Sub( function_name ) );
	if ( func_type == CONSTRUCTOR ) {
		writeFunction( ofp, classdotfunc, var_SubrotCount( "var" ) );
		char tmp_token[256];
		strcpy( tmp_token, token );
		sprintf( token, "%d", var_ClassCount("field") );
		writePush( ofp, VM_CONST, var_ClassCount("field") );
		strcpy( token, tmp_token );
		if ( strcmp( function_name, "new" ) == 0 ){
			if ( debug || isstdout ) {
				fprintf( stdout, "call Memory.alloc 1\n" );
			} else {
				fprintf( ofp, "call Memory.alloc 1\n" );
				writePop( ofp, VM_POINTER, 0 );
			}
		}
	} else if ( func_type == METHOD ) {
		// TODO:これで本当にあってる？
		writeFunction( ofp, classdotfunc, var_SubrotCount( "var" ) );
		writePush( ofp, VM_ARG, 0 );
		writePop( ofp, VM_POINTER, 0 );
	} else if ( func_type == FUNCTION ) {
		writeFunction( ofp, classdotfunc, var_SubrotCount( "var" ) );
	}

	// statementsをコンパイル
	compile_Statements( ifp, ofp, thd_depth, function_type, if_index, while_index );
	compile_Symbol( ifp, ofp, '}', thd_depth );

	printTokenAndTagEnd( ofp, "subroutineBody", sec_depth );

	printTokenAndTagEnd( ofp, "subroutineDec", depth );
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	// vmコードで関数名を生成しておく。

	return 1;
}

int compile_Statements( FILE * ifp, FILE * ofp, int depth, char func_type[256], int if_id, int while_id ) {
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

			if ( debug ) {
				fprintf( stdout, "[%s]: token is %s\n", __func__, token );
			}
			if ( type_of_token == KEYWORD ) {
				if ( strcmp( token, "let" ) == 0 ) {

					compile_Let_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "if" ) == 0 ) {

					compile_If_Statement( ifp, ofp, sec_depth, if_index, while_index );

				} else if ( strcmp( token, "while" ) == 0 ) {

					compile_While_Statement( ifp, ofp, sec_depth, if_index, while_index );

				} else if ( strcmp( token, "do" ) == 0 ) {

					compile_Do_Statement( ifp, ofp, sec_depth );

				} else if ( strcmp( token, "return" ) == 0 ) {

					compile_Return_Statement( ifp, ofp, sec_depth, func_type );

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
			break;
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

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
			symbol_define = 1;
			strcpy( propof, token );
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
			strcpy( my_typeof, token );
			printTokenAndTag( ofp, t_type, token, sec_depth );
		} else 	if ( list_Find_Node( token ) ) {
			// プリミティブ型以外の場合
			strcpy( my_typeof, token );
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
			my_define( 0, token, my_typeof, propof, cnt_var );
			cnt_var += 1;
			//printTokenAndTag( ofp, t_type, token, sec_depth );
			printTokenStatus( ofp, token, sec_depth+1 );
		} else {
			return -1;
		}
	}

	// 区切り文字;が来るまでコンパイルを続ける
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
				if ( strcmp( propof, "var" ) == 0 ) {
					my_define( 0, token, my_typeof, propof, cnt_var );
					cnt_var += 1;
					//printTokenAndTag( ofp, t_type, token, sec_depth );
					printTokenStatus( ofp, token, sec_depth+1 );
				}
			} else {
				fprintf( stdout, "[ERROR] next token must be identifier\n" );
				return -1;
			}
		} else {
			fprintf( stdout, "[ERROR] no more any tokens\n" );
			return -1;
		}
	}

	symbol_define = 0;
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

	// サブルーチン名をコンパイルする
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		depth++;

		type_of_keyword = token_type( token );
		if ( type_of_keyword == IDENTIFIER ) {
			printTokenAndTag( ofp, t_type, token, depth );
			list_t * p = list_Find_Node_Subrot_BelongClass( token );
			if ( p ) {
				printSubrotStatus( ofp, p, token, depth );
			}
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

			// 引数の型をコンパイルする
			if ( type_of_token == KEYWORD ) {
				// プリミティブ型の場合
				if ( 	strcmp( token, "int" ) == 0 
					|| strcmp( token, "char " ) == 0 
					|| strcmp( token, "boolean" ) ) {
					strcpy( my_typeof, token );		
					printTokenAndTag( ofp, t_type, token, sec_depth );
				} else {
					ungets( ifp, strlen( token ) );
					break;
				}
			} else if ( type_of_token == IDENTIFIER ) {
				// プリミティブ型以外の型の場合
				strcpy( my_typeof, token );
				printTokenAndTag( ofp, t_type, token, sec_depth );
				// 型のため出力はしない
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
				strcpy( propof, "argument" );
				my_define( 0, token, my_typeof, propof, cnt_arg );
				cnt_arg += 1;
				//printTokenAndTag( ofp, t_type, token, sec_depth );
				printTokenStatus( ofp, token, depth );
			}
		} else {
			break;
		}
		if ( compile_Symbol( ifp, ofp, ',', sec_depth ) ) {
			// goto next parameter		
			// 他に引数がある場合, ループをブレイクしない
			;
		} else if ( compile_Symbol( ifp, ofp, ';', sec_depth ) ) {
			break;
		}
	}
	printTokenAndTagEnd( ofp, "parameterList", depth );
	return 0;
}

int compile_Let_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int flag = 0;
	char before;
	int sec_depth = depth+1;
	char thistoken[256];
	scope_t * tmp;

	printTokenAndTagStart( ofp, "letStatement", depth );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	// 変数名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		strcpy( thistoken, token );
		if ( type_of_token == IDENTIFIER ) {
			strcpy( propof, "notVar" );
			//printTokenAndTag( ofp, t_type, token, sec_depth );
			printTokenStatus( ofp, token, sec_depth );
			tmp = list_Find_Scope( token );
			tmp->used = 1;
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
		fprintf( stdout, "[%s]:token %s kind is %d", __func__, thistoken, kind_Of(thistoken) );
	}	

	if ( kind_Of( thistoken ) == VAR ) {
		writePop( ofp, VM_LOCAL, index_Of( thistoken) );
		//writePush( ofp, VM_LOCAL, index_Of( thistoken) );
	} else if ( kind_Of( thistoken ) == ARG ) {
		writePop( ofp, VM_ARG, index_Of( thistoken ) );
	} else if ( kind_Of( thistoken ) == FIELD ) {
		writePop( ofp, VM_THIS, index_Of( thistoken ) );
	}


	printTokenAndTagEnd( ofp, "letStatement", depth );
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}


	return 1;
}

void compile_If_Statement( FILE * ifp, FILE * ofp, int depth, int if_id, int while_id ) {
	int type_of_token;
	int sec_depth = depth+1;
	char if_true[256];
	char if_false[256];
	char if_end[256];

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "ifStatement", depth );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	compile_Symbol( ifp, ofp, '(', sec_depth );
	// 条件文をコンパイルする
	compile_Expression( ifp, ofp, sec_depth );

	
	// if文vmコードへ変換する
	sprintf( if_true, "IF_TRUE%d", if_index ); 
	sprintf( if_false, "IF_FALSE%d", if_index );
	sprintf( if_end, "IF_END%d", if_index  );
	if_index++;

	compile_Symbol( ifp, ofp, ')', sec_depth );

	compile_Symbol( ifp, ofp, '{', sec_depth );

	writeIf( ofp, if_true ); // 条件が真の場合, if-trueへジャンプ
	writeGoto( ofp, if_false ); // 条件が偽の場合, if-falseへジャンプする
	writeLabel( ofp, if_true );

	// 条件分岐後の式をコンパイルする
	compile_Statements( ifp, ofp, sec_depth, "", if_index, while_index );
	compile_Symbol( ifp, ofp, '}', sec_depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );

		if ( type_of_token == KEYWORD ) {
			// else文をコンパイルする
			if ( strcmp( token, "else" ) == 0 ) {
				printTokenAndTag( ofp, t_type, token, sec_depth );
				if ( compile_Symbol( ifp, ofp, '{', sec_depth ) ) {
					writeLabel( ofp, if_false ); 
					// 条件分岐後の式をコンパイルする
					compile_Statements( ifp, ofp, sec_depth, "", if_index, while_index );

					compile_Symbol( ifp, ofp, '}', sec_depth );
				}
			} else {
				writeLabel( ofp, if_false ); 
				ungets( ifp, strlen( token ) );
				
			}
		} else {
			ungets( ifp, strlen( token ) );
		}
	}

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
	printTokenAndTagEnd( ofp, "ifStatement", depth );
}

void compile_While_Statement( FILE * ifp, FILE * ofp, int depth, int if_id, int while_id ) {
	int type_of_token;
	int sec_depth = depth+1;
	char while_start[256];
	char while_end[256];
	snprintf( while_start, 256, "WHILE_EXP%d", while_index );
	snprintf( while_end, 256, "WHILE_END%d", while_index ); 
	while_index++;

	// whileループのスタート部分のラベルを出力する
	writeLabel( ofp, while_start ); 
	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}
	printTokenAndTagStart( ofp, "whileStatement", depth );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	compile_Symbol( ifp, ofp, '(', sec_depth );

	// while文の条件部分をコンパイルする
	compile_Expression( ifp, ofp, sec_depth );
	writeArithmetic( ofp, "not" );

	compile_Symbol( ifp, ofp, ')', sec_depth );

	compile_Symbol( ifp, ofp, '{', sec_depth );

	// if-goto while_end 
	writeIf( ofp, while_end );
	// while文の本文をコンパイルする
	compile_Statements( ifp, ofp, sec_depth, "", if_index, while_index );

	writeGoto( ofp, while_start );
	compile_Symbol( ifp, ofp, '}', sec_depth );

	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	writeLabel( ofp, while_end );
	printTokenAndTagEnd( ofp, "whileStatement", depth );

}

void compile_Do_Statement( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;

	list_t * p = list_Find_Node( token );
	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__  );
	}

	printTokenAndTagStart( ofp, "doStatement", depth );
	printTokenAndTag( ofp, t_type, token, sec_depth );

	// サブルーチンをコンパイルする
	compile_Subroutine_Call( ifp, ofp, p, sec_depth );
	compile_Symbol( ifp, ofp, ';', sec_depth ); 

	// fprintf( stdout, "in %s \n", __func__ );
	writePop( ofp, VM_TEMP, 0 );
	// fprintf( stdout, "out %s\n", __func__ );


	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTokenAndTagEnd( ofp, "doStatement", depth );
}

void compile_Subroutine_Call( FILE * ifp, FILE * ofp, list_t * class_pos, int depth ) {
	int type_of_token;
	char class_name[256];
	char tmp_token[256];
	char classdotfunc[256];
	int is_thisclassmethod = 0;
	subroutine_name_t * p;
	scope_t * class_var;
	list_t * lp;
	strcpy( class_name, token );
	int argnum = 0;
	int var_class = 0;
	int is_access_obj_field = 0;

	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__  );
	}

	// サブルーチン名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "[%s]: token is %s\n", __func__, token );
		}
		if ( type_of_token == IDENTIFIER ) {
			printTokenAndTag( ofp, t_type, token, depth );
			strcpy( tmp_token, token );
			advance( ifp );
			if ( token[0] == '(' ) {
				lp = list_Find_Node_Subrot_BelongClass( tmp_token );
				if ( !lp ) {
					if ( debug ) {
						fprintf( stdout, "this function is not registered at list, %s\n", tmp_token );
					}
					lp = list_Find_Node( thisclassname );
					list_Add_Subrot( lp, tmp_token );	
					// list_Print();
				}
				printSubrotStatus( ofp, lp, tmp_token, depth );
				ungets( ifp, strlen( token ) );
				// 関数コール用の文字列をここで作成する
				sprintf( classdotfunc, "%s.%s", thisclassname, tmp_token ); 
				is_thisclassmethod = 1;
				is_access_obj_field = 1;
			} else if ( token[0] == '.' ) {
				// 他のクラスメソッドの場合, クラス名をコンパイルすることになる

				class_var = list_Find_Scope( tmp_token );
				if ( class_var ) {
					// tmp_token の値がクラス名でなく, あるクラスの変数名であった場合
					if ( debug ) {
						fprintf( stdout, "this var class type is %s\n", class_var->type  );
					}
					var_class = 1;
					if ( strcmp( class_var->proper, "var" ) == 0 ) {
						writePush( ofp, VM_LOCAL, class_var->number );
					} else {
						writePush( ofp, VM_THIS, 0 );	
					}
					is_thisclassmethod = 1;
					// sprintf( thisclassname, "%s", class_var->type );
				} else {
					lp = list_Find_Node( tmp_token );
					if ( !lp ) {
						// TODO:tmp_token`が必ずしもクラス名とは限らない
						// 　　:変数名かもしれないので, その変数名があるかを確認する必要がある
						// クラス名がリストに未登録の場合
						list_Add( tmp_token );
						lp = list_Find_Node( tmp_token );
						if ( debug ) {
							fprintf( stdout, "this class isnot registerd at class list\n" );
						}
					} else {
						if ( debug ) {
							fprintf( stdout, "this class is registerd at class list %s\n",tmp_token );
						}
					}
				}
				printClassStatus( ofp, lp, tmp_token, depth );
				ungets( ifp, strlen( token ) );
			}
		}
	}

	// (をコンパイル
	if ( compile_Symbol( ifp, ofp, '(', depth ) ) {
		// サブルーチンの引数をコンパイル
		if ( is_thisclassmethod ) {
			argnum = compile_Expression_List( ifp, ofp, depth ) + 1;
		} else {
			argnum = compile_Expression_List( ifp, ofp, depth );
		}
		compile_Symbol( ifp, ofp, ')', depth );
	} else if ( compile_Symbol( ifp, ofp, '.', depth ) ) {
		// 他クラスのメソッドの場合, ここの部分でメソッド名コンパイルする
		if ( has_more_tokens( ifp) ) {
			advance( ifp );
			type_of_token = token_type( token );

			if ( debug ) {
				fprintf( stdout, "call subroutine, this method is registered at method list\n" );
			}

			// printTokenAndTag( ofp, t_type, token, depth );
			if ( type_of_token == IDENTIFIER ) {
				// methodの登録クラスを確認
				if ( debug ) {
					fprintf( stdout, "token is   : %s\n", token );
				}
				if ( var_class == 1 ) {
					//list_Print();
					if ( debug ) {
						fprintf( stdout, "this method belongs to %s\n", class_var->type  );
					}
					is_thisclassmethod = 1;
					strcpy( tmp_token, class_var->type );		
				} else { 
					lp = list_Find_Node( tmp_token );
					if ( debug ) {
						fprintf( stdout, "method belonging class is [%s], address is [%p]\n", tmp_token, lp );
					}
					// 登録クラス中のメソッドが登録されているか確認
					p = list_Find_Node_Subrot( lp, token );

					if ( debug ) {
						fprintf( stdout, "method address is %p, %s\n", p->subroutine_name, token );
					}
					if ( p ) {
						// lp = list_Find_Node_Subrot_BelongClass( token );
						printSubrotStatus( ofp, lp, token, depth );
					} else {
						list_Add_Subrot( lp, token );
						p = list_Find_Node_Subrot( lp, token );
						printSubrotStatus( ofp, lp, token, depth );
					}
				}
			}
			sprintf( classdotfunc, "%s.%s", tmp_token, token );

			if ( compile_Symbol( ifp, ofp, '(', depth ) ) {
				if ( is_thisclassmethod ) {
					argnum = compile_Expression_List( ifp, ofp, depth ) + 1;
				} else {
					argnum = compile_Expression_List( ifp, ofp, depth );
				}
				compile_Symbol( ifp, ofp, ')', depth );
			} else {
				fprintf( stdout, "[ERROR]: Subroutine Parameter must start (, %s\n", token );
				return;
			}
		}
	}

	if ( is_access_obj_field == 1 ) {
		writePush( ofp, VM_POINTER, 0 );
	}

	writeCall( ofp, classdotfunc, argnum );
	
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}
}

int compile_Return_Statement( FILE * ifp, FILE * ofp, int depth, char func_type[256] ) {
	int type_of_token;
	int sec_depth = depth+1;
	int flag = 0;
	char tmp_token[256];

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "returnStatement", depth );
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

	if ( strcmp( func_type, "void" ) == 0 ) {
		if ( isstdout & !xml ) {
			// writePop( ofp, VM_TEMP, 0 );
			// void時には0を返すため，一度0の値をtokenへコピー
			// 一次退避用にtmp_tokenへtokenの値の保存及び復元を行う
			strcpy( tmp_token, token );	
			strcpy( token, "0" );
			writePush( ofp, VM_CONST, 0 );
			strcpy( token, tmp_token );
		} else if ( !isstdout & !xml ){
			// fprintf( ofp, "pop temp 0\n" );
			fprintf( ofp, "push constant 0\n" );
		}
	}

	printTokenAndTagEnd( ofp, "returnStatement", depth );
	writeReturn( ofp );
	return 1;
}

void compile_Expression( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int sec_depth = depth+1;
	int flag = 0;
	list_t * clsp;
	char op_token[256];

	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__  );
	}

	// tokenの属性判定
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if (	type_of_token == INT_CONST || type_of_token == STRING_CONST || 
			strcmp( token, "true" ) == 0 || strcmp( token, "false" ) == 0 || 
			strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {
			// 定数
			flag = 1;
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			// 変数の属性などは下で出力する
			flag = 2;
		} else if ( type_of_token == SYMBOL ) {
			if ( token[0] == '(' || token[0] == '-' || token[0] == '~' ) {
				flag = 3;
				strcpy( op_token, token );
			}
		}
	}

	// token名をコンパイル, 属性に応じて出力先を変更する
	if ( flag > 0 ) {
		ungets( ifp, strlen( token ) );
		printTokenAndTagStart( ofp, "expression", depth );
		if ( flag == 2 ) {
			// tokenがクラス, サブルーチン, 変数の時でプリント関数を切り替える
			if ( ( clsp = list_Find_Node( token ) ) ) {
				printClassStatus( ofp, clsp, token, depth );
			} else if ( ( clsp = list_Find_Node_Subrot_BelongClass( token ) ) ) {
				printSubrotStatus( ofp, clsp, token, depth );
			} else {
				// fprintf( stdout, "debug information\n" );
				//printTokenStatus( ofp, token, depth );
			}
		} 
		compile_Term( ifp, ofp, sec_depth );

		while ( 1 ) {
			if ( 	   compile_Symbol( ifp, ofp, '+', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '-', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '*', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '/', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '&', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '|', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '<', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '>', sec_depth ) 
				|| compile_Symbol( ifp, ofp, '=', sec_depth ) ) {
				
				strcpy( op_token, token );
				compile_Term( ifp, ofp, sec_depth );
				writeArithmetic( ofp, op_token );
			} else {
				break;
			}
		}

		if ( debug ) {
			fprintf( stdout, "[%s]:Finish\n", __func__ );
		}

		printTokenAndTagEnd( ofp, "expression", depth );

	} else {
		return;
	}
}

void compile_Term( FILE * ifp, FILE * ofp, int depth ) {
	int type_of_token;
	int flag = 0;
	int sec_depth = depth+1;
	char tmp_symbol[256];
	char tmp_token[256];
	list_t * p;

	if ( debug ) {
		fprintf( stdout, "[%s]\n", __func__  );
	}

	printTokenAndTagStart( ofp, "term", depth );

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );

		type_of_token = token_type( token );
		if ( debug ) {
			fprintf( stdout, "[%s]: char is %c\n", __func__, token[0] );
		}

		if ( type_of_token == INT_CONST ) {
			// integerConst
			flag = 1;
			printTokenAndTag( ofp, t_type, token, sec_depth );

			writePush( ofp, VM_CONST, index_Of( token ) );
		} else if ( type_of_token == STRING_CONST ) {
			// stringConst
			// "はシンボルではないので, 下に書いた部分では引っかからない
			flag = 1;
			get_stringconst( ifp );
			// printTokenAndTag( ofp, t_type, token, sec_depth );
			writePush( ofp, VM_CONST, strlen( token ) );
			// メソッドの場合は引数のベースアドレスを1からスタートする
			fprintf( ofp, "call String.new 1\n" );
		} else if (	strcmp( token, "true" ) == 0 || strcmp( token, "false" ) == 0 || 
				strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {
			// keywordConst
			flag = 1;
			printTokenAndTag( ofp, t_type, token, sec_depth );

			// 値を退避しつつ行う
			strcpy( tmp_symbol, token );
			strcpy( token, "0" );

			if ( strcmp(tmp_symbol, "true" ) == 0 ) {
				writePush( ofp, VM_CONST, 0 );
				strcpy( token, tmp_symbol );
				writeArithmetic( ofp, "not" );
			} else if ( strcmp( tmp_symbol, "this" ) == 0 ) {
				strcpy( token, tmp_symbol );
				writePush( ofp, VM_POINTER, 0 );
			} else if ( strcmp( tmp_symbol, "false" ) == 0 || strcmp( tmp_symbol, "null") == 0 ) {
				writePush( ofp, VM_CONST, 0 );
				strcpy( token, tmp_symbol );
			} 
		
		} else if ( type_of_token == IDENTIFIER ) {
			// varName
			if ( ( p = list_Find_Node( token ) ) ) {
				// サブルーチン呼び出し
				flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Subroutine_Call( ifp, ofp, p, sec_depth );
			} else {
				flag = 1;
				//printTokenAndTag( ofp, t_type, token, sec_depth );
				printTokenStatus( ofp, token, sec_depth );
				strcpy( tmp_token, token );
				if ( compile_Symbol( ifp, ofp, '[', sec_depth ) ) {
					// 変数が配列の場合
					compile_Expression( ifp, ofp, sec_depth );
					compile_Symbol( ifp, ofp, ']', sec_depth );
				} else {
					int inumber = index_Of( tmp_token );
					int tnumber = kind_Of( tmp_token );
					// fprintf( stdout, "type number is %d\n", tnumber );
					if ( tnumber == VAR ) {
						writePush( ofp, VM_LOCAL, inumber );
					} else if ( tnumber == ARG ) {
						writePush( ofp, VM_ARG, inumber );
					} else if ( tnumber == FIELD ) {
						writePush( ofp, VM_THIS, inumber );
					}
				}
			}
		} else if ( type_of_token == SYMBOL ) {
			// fprintf( stdout, "this is symbol\n" );
			strcpy( tmp_symbol, token );
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
				writeArithmetic( ofp, "neg" );
			} else if ( token[0] == '~' ) {
				flag = 1;
				ungets( ifp, strlen( token ) );
				compile_Symbol( ifp, ofp, '~', sec_depth );
				compile_Term( ifp, ofp, sec_depth );
				writeArithmetic( ofp, "not" );
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
		fprintf( stdout, "[%s]:Start\n", __func__ );
	}
	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		type_of_token = token_type( token );
		fprintf( stdout, "first token value is %d\n", token[0] );
		if ( type_of_token == SYMBOL && token[0] == sym ) {
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
	int expression_num = 0;

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
			// 下のExpression部分で出力する
			flag = 1;
		} else if ( type_of_token == SYMBOL ) {
			if ( token[0] == '-' || token[0] == '~' || token[0] == '(' ) {
				flag = 1;
			}
		}

		ungets( ifp, strlen( token ) );
		if ( flag == 1 ) {
			compile_Expression( ifp, ofp, sec_depth );
			expression_num++;
			while ( compile_Symbol( ifp, ofp, ',', sec_depth )  ) {
				compile_Expression( ifp, ofp, sec_depth );
				expression_num++;
			}
		}
	}
	if ( debug ) {
		fprintf( stdout, "[%s]:Finish\n", __func__ );
	}

	printTokenAndTagEnd( ofp, "expressionList", depth );

	return expression_num;
}
