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
		compile_class( ifp );
	}
}

int compile_Class( FILE * ifp ) {

	int type_of_token;
	fprintf( stdout, "<class>\n" );
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token ); 
		if ( strcmp( token, "Class" ) == 0 && type_of_token == KEYWORD ) {
			fprintf( stdout, "\t<keyword> %s </keyword>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Program must starts \"Class\", compile canceled\n" );
			return;
		}
	} 

	// クラス名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Class name must be identifier\n" );
			return;
		}
	}

	if ( !compile_Symbol( ifp, '{' ) ) {
		fprintf( stdout, "[ERROR]: After class name, { was expected\n" );
		return;
	}


	// 大きめおサブルーチンを作成してそちらに処理を
	// すべて書いた方が可読性が上がる気がする
	fprintf( stdout, "<classVarDec>\n" );
	compile_Class_Var_Dec();
	fprintf( stdout, "</subroutineDec>\n" );


	// 上記と同様
	fprintf( stdout, "<subroutineDec>\n" );
	compile_subroutine_var_dec();
	fprintf( stdout, "</subroutineDec>\n" );

	if ( !compile_Symbol( '}' ) ) {
		fprintf( stdout, "[ERROR]: After class name, { was expected\n" );
		return;
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

// class_var_decをコンパイルする
int compile_Class_Var_Dec( ifp )  {

	int type_of_token;

	// static or field
	if ( has_more_tokens( ifp ) ) {

		advance();
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "static" ) == 0 || strcmp( token, "field" ) == 0 ) ) {
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Class var declaration must start static of field\n" );
		}
	}

	// 型のコンパイル
	if ( has_more_tokens( ifp ) ) {

		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			} else {
				fprintf( stdout, "[ERROR]: Variable type must be int, char or boolean\n" );
			}
		} else {
			fprintf( stdout, "[ERROR]: Var type must be int, char or boolean\n" );
		}
	}

	// 区切り文字が, である限り変数名のコンパイルを続ける
	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			// 変数名をコンパイル
			advance();

			type_of_token = token_type( token );
			if ( type_of_token == IDENTIFIER ) {
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			} else {
				fprintf( stdout, "[ERROR]: Variable name must be identifier\n" );
			}
		}


		if ( compile_Symbol( ifp, ',') ) {
			; // do nothing
		} else if ( compile_Symbol( ifp, ';' ) ) {
			break;
		} else {
			fprintf( stdout, "[ERROR]: Var name next token is ; or , \n" );
			break;
		}
	}
}

int compile_Subroutine_Dec( FILE * ifp ) {
	int type_of_token;


	// サブルーチンの最初のトークンはconstructor, function, methodのいづれかで始まる
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "constructor" ) == 0 || strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			} else {
				fprintf( stdout, "[ERROR]: First token must be constructor, function or method\n" );
			}
		}
	}

	// サブルーチンの型はvoid, int, char, boolean 及びクラス名のいずれか
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( 	strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || 
				strcmp( token, "boolean" ) == 0 || strcmp( token, "void" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			} else {
				fprintf( stdout, "[ERROR]: Function type must be int, char or boolean\n" );
			}
		}
	}

	// サブルーチン名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		} else {
			fprintf( stdout, "[ERROR]: Function name must be identifier\n" );
		}
	}

	// サブルーチン名の後のシンボルは(が来る
	if ( !compile_Symbol( ifp, '(' ) ) {
		fprintf( stdout, "[ERROR]: After subroutime name must be (\n" );
		return;
	}

	// パラメータリストをコンパイル
	compile_parameterlist( ifp );

	if ( !compile_Symbol( ifp, ')' ) ) {
		return;
	}

	// サブルーチン本体をコンパイル
	// はじめのトークンは{
	compile_Symbol( ifp, '{' );

	// VarDecをコンパイル TODO:別関数として作成すること
	compile_Var_Dec( ifp );

	// statementsをコンパイル
	compile_Statements( ifp );

}

int compile_Statements( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );

		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "let" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
				compile_Let_Statement( ifp );
			} else if ( strcmp( token, "if" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
				compile_If_Statement( ifp );
			} else if ( strcmp( token, "while" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
				compile_While_Statement( ifp );
			} else if ( strcmp( token, "do" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
				compile_Do_Statement( ifp );
			} else if ( strcmp( token, "return" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
				compile_Return_Statement( ifp );
			}
		}		
	}

}

int compile_Var_Dec( FILE * ifp ) {
	/* var type varname(identifier) (, varname)* ;*/
	int type_of_token;

	// var 
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && strcmp( token, "var" ) == 0 ) {
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		}
	}

	// type 
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD && ( strcmp( token, "int" ) == 0 || strcmp( token, "char" ) == 0 || strcmp( token, "boolean" ) == 0 ) ) {
			fprintf( stdout, "<keyword> %s </keyword>\n", token );
		}
	}

	// varname 
	if ( has_more_tokens( ifp ) ) {
		advane();

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %S </identifier>\n", token );
		}
	}


	if ( !compile_Symbol( ifp, '(' ) ) {
		return;
	}

	while ( 1 ) {
		// 
		if ( compile_Symbol( ifp, ',' ) ) {
			; // do nothing
		} else if ( compile_Symbol( ifp, ';' ) ) {
			break;
		}

		if ( has_more_tokens( ifp ) ) {
			advance();

			type_of_token = token_type( token );
 			if ( type_of_token == IDENTIFIER ) {
				fprintf( stdout, "<identifier> %s </identifier>\n", token );
			}
		}
	}
}


void compile_subroutine () {
	int type_of_keyword;

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		
		token_of_number = token_type( token );
		if ( token_of_number == KEYWORD ) {
			if ( strcmp( token, "constructor" ) == 0 || strcmp( token, "function" ) == 0 || strcmp( token, "method" ) == 0 ) { 
				fprintf( stdout, "<keyword> %s </keyword>\n" );
				compile_var_type( ifp );
				compile_subroutine_name( ifp );
				compile_Symbol( ifp, '(' );
				compile_parameterlist( ifp );
				compile_Symbol( ')' );
			}  else {
				fprintf( stdout, "[ERROR]: Next token is keyword( constructor, function, mehtod) is expected\n" );
			}
		} 
	} 
}	

int compile_var_name( FILE * ifp ) {
	int type_of_keyword;

	if( has_more_tokens( ifp ) ) {
		advance( ifp );
		
		type_of_keyword = token_type( token );
		if( type_of_keyword == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Var name is identifier, %s\n", __func__ );
		}
	}
	return 0;
}


void unegets( FILE * ifp ) {
	for ( int i = 0 ; token[i] != '\0' ; i++ ) {
		ungetc( token[i], ifp );
	}
}

int compile_subroutine_name( FILE * ifp ) {
	int type_of_keyword;

	if ( has_more_tokens( ifp ) ) {
		advance( ifp );
		
		type_of_keyword = token_type( token );
		if ( type_of_keyword == IDENTIFIER ) {
 			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			return 1;
		} else {
			fprintf( stdout, "[ERROR]: Next token must be identifier, subroutine name\n" );
		}
	}

	ungets( ifp );
	return 0;
}

int compile_parameterlist( FILE * ifp ) {

	int token_of_number;

	while ( 1 ) {
		compile_var_type( ifp );
		compile_var_name( ifp );
		if ( compile_Symbol( ifp, ',') ) {
			// goto next parameter		
		} else if ( compile_Symbol( ifp, ';' ) ) {
			break;
		}
	}

	return 0;
}

int compile_statements( FILE * ifp ) {
	
	if ( has_more_token ( ifp ) ) {
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

int compile_do( FILE * ifp ) {

	int type_of_token;

	// subroutinename
	if ( has_more_token( ifp ) ) {
		advance();
		
		// next token is subroutinename, identifier 
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

}

int compile_subroutine( FILE * ifp ) {
	int type_of_token;

	// subroutinename 
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

	// symbol
	compile_Symbol( ifp, '(' );

	compile_expressionlist( ifp );
}

int compile_expreassionlist( FILE * ifp ) {
	int type_of_token;


	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
		
			compile_expression( ifp );
		}
	}

}

void compile_Let_Statement( FILE * ifp ) {
	int type_of_token;
	int flag = 0;
	char before;

	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "let" ) == 0 ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );
			}
		}
	}

	// 変数名をコンパイル
	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );
		if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		}
	}

	// TODO ; beforeへの値いれを行う
	compile_Symbol( ifp, '=' );
	
	if ( before == '[' ) {
		compile_Expression( ifp );
		if ( has_more_tokens( ifp ) ) {
			advance();

			type_of_token = token_type( token );
			if ( has_more_tokens( ifp ) ) {
				advance();

				if ( type_of_token == SYMBOL ) {
					fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
				}
			}
		}
	}


	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '=' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	} 	


	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );

		if ( type_of_token == SYMBOL ) {
			if ( token[0] == ';' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}
}

void compile_If_Statement( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '(' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}

	compile_Expression( ifp );

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == ')' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}

	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '{' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}
	
	compile_Statements( ifp );

	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_tokens = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '}' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == KEYWORD ) {
			if ( strcmp( token, "else" ) ) {
				fprintf( stdout, "<keyword> %s </keyword>\n", token );

				if ( has_more_token( ifp ) ) {
					advance();
					type_of_token = token_type( token );
					if ( type_of_token == SYMBOL ) {
						if ( token[0] == '{' ) {
							fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
							compile_Statements( ifp );

							if ( has_more_token( ifp ) ) {
								advance();
								type_of_token = token_type( token );
								if ( type_of_token == SYMBOL ) {
									if ( toke[0] == '}' ) {
										fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
									}
								}
							}
						}
					} 
				}
			} else {
				ungets( ifp );
			}
		}
	}
}

void compile_While_Statement( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '(' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}

	compile_Expression( ifp );

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == ')' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '{' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}
	
	compile_Statements( ifp );

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '}' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			}
		}
	}
	
}

void compile_Do_Statement( FILE * ifp ) {
	  int type_of_token;
	
	  compile_Subroutine_Call( FILE * ifp );
}

void compile_Subroutine_Call( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		  advance();

		  type_of_token = token_type( token );
		  if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
		  }
	}

	
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL ) {
			if ( token[0] == '(' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
				compile_Expression_List( ifp );

				if ( has_more_tokens( ifp ) ) {
					advance();

					type_of_token = token_type( token );
					if ( type_of_token == SYMBOL ) {
						fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
					}
				}
			} else if ( token[0] == '.' ) {
				fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );

				if ( has_more_tokens( ifp ) ) {
					advance();

					type_of_tokens = token_type( token );
					if ( type_of_token == IDENTIFIER ) {
						fprintf( stdout, "<identifier> %s </identifier>\n", token );
					}

					if ( has_more_tokens( ifp ) ) {
						advance();

						type_of_token = token_type( token );
						if ( type_of_token == SYMBOL && token[0] == '(' ) {
							fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
						}

						compile_Expression_List( ifp );

						if ( has_more_tokens( ifp ) ) {
							advance();

							type_of_token = token_type( token );
							if ( type_of_token == SYMBOL && token[0] == '}' ) {
								fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
							}
						} 
					}
				}

			}
		}
	}
}

void compile_Return_Statement( FILE * ifp ) {
	int type_of_token;
	
	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL && token[0] == ';' ) {
			fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
		} else {
			compile_Expression( ifp );

			if ( has_more_tokens( ifp ) ) {
				advance();

				type_of_token = token_type( token );
				if ( type_of_token == SYMBOL && token[0] == ';' ) {
					fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
				}
			}
		}
	}
}

void compile_Expression( FILE * ifp ) {
	int type_of_token;

	compile_Term( ifp );

	while ( 1 ) {
		if ( has_more_tokens( ifp ) ) {
			advance();

			type_of_token = token_type( token );
			if ( type_of_token == SYMBOL ) {
				if ( token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/' 
						|| token[0] == '&' || token[0] == '|' || token[0] == '<' || token[0] == '>' || token[0] == '=' ) {
					fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
					compile_Term( ifp );
				} else {
					break;
				}
			}
		}
	}
}

void compile_Term( FILE * ifp ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();

		type_of_token = token_type( token );
		if ( type_of_token == INT_CONST ) {
			fprintf( stdout, "<integerconst> %d </integerconst>\n", atoi( token ) );
		} else if ( type_of_token == STRING_CONST ) {
			fprintf( stdout, "<stringconst> %s </stringconst>\n", token );
		} else if ( strcmp( token, "true" ) == 0 || strcmp( token, "false" ) == 0 || strcmp( token, "null" ) == 0 || strcmp( token, "this" ) == 0 ) {
			fprintf() stdout, "<keywordconst> %s <keywordconst>\n", token );
		} else if ( type_of_token == IDENTIFIER ) {
			fprintf( stdout, "<identifier> %s </identifier>\n", token );
			if ( has_more_tokens( ifp ) ) {
				advance();
				
				int tmp_type_of_token;
				tmp_type_of_token = token_type( token );
				if ( tmp_type_of_token == '[' ) {
					compile_Expression( ifp );
				}
			}
		}
	}
}

int compile_Symbol( FILE * ifp, char sym ) {
	int type_of_token;

	if ( has_more_tokens( ifp ) ) {
		advance();
		type_of_token = token_type( token );
		if ( type_of_token == SYMBOL && token[0] == sym ) {
			fprintf( stdout, "<symbol> %c </symbol>\n", token[0] );
			return 1;
		} else {
			ungetc( token[0], ifp  );
			return 0;
		}
	}
	return 0;
} 
