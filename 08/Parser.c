#include "define.h"

// 入力ファイルへのファイルポインタ
FILE * fp;
// 入力ファイルのファイルネーム
char filename[256];
// 入力ストリーム
char line[256];
// 現在のコマンド
char current_cmd[256];
// コマンドのみを格納する
char cmd[256];
// 第一引数
char argstr1[256];
// 第二引数
char argstr2[256];
// スタック領域

// 出力先ファイルへのファイルポインタ
FILE * outputfp;
// 出力先ファイルネーム
char outputfilename[1000];
// .vm除去したもの
char fnameex2[256];

unsigned int eqnum = 0;
unsigned int ltnum = 0;
unsigned int gtnum = 0;

int main( int argv, char ** argc ) {
	InitAll();
	strcpy( filename, argc[1] );
	fprintf( stdout, "%s\n", filename );
	makeOutputFilename();
	fprintf( stdout, "ParserMain Start\n" );
	ParseMain();
	return 0;	
}

// TODO: 関数名をParseMainから変更する必要あり
//     : 関数内部での処理がparse以外のことも行なっているため
//     : PasereMain -> VMTransMainにする
void ParseMain() {
	int type = 0, arg2c = 0;
	fp = fopen( filename, "r" ); 

	setFileName( "a.asm" );
	
	// 初期化用のアセンブラコードは必ず最初に呼ばれる
	//	writeInit();

	// 入力ストリームの文字列を最後まで読む
	while ( hasMoreCommands() ) {
		// PrintAscii( line );
		advance();
		type = commandType();

		if ( type != -1 ) {
			fprintf( outputfp, "//%s", line  );
			fprintf( stdout, "%s", cmd );
			// 1番目の引数をargstr1に格納する
			if ( type != C_RETURN ) {
				arg1();
				fprintf( stdout, " %s", argstr1 );
			}

			// 2番目の引数をargstr2に格納する
			arg2c = 0;
			if ( type == C_PUSH || type == C_POP || type == C_FUNCTION || type == C_CALL ) {
				arg2c = arg2();
				fprintf( stdout, " %d", arg2c );
			}
			fprintf( stdout, "\n" );	

			// ファイル書き込み用
			if ( type == C_POP || type == C_PUSH ) {
				writePushPop( type, argstr1, atoi( argstr2 ) );
			} else if ( type == C_ARITHMETIC ) {
				writeArithmetic( cmd );
			} else if ( type == C_GOTO ) {
				writeGoto( argstr1 );
			} else if ( type == C_LABEL ) {
				writeLabel( argstr1 );
			} else if ( type == C_IF ) {
				writeIf( argstr1 );
			} else if ( type == C_FUNCTION ) {
				writeFunction( argstr1, atoi( argstr2 ) );
			} else if ( type == C_RETURN ) {
				writeReturn();
			} else if ( type == C_CALL ) {
				writeCall( argstr1, atoi( argstr2 ) );
			}
		}
	}
	close();
	fclose( fp );
}

bool hasMoreCommands() {
		
	if ( fgets( line, sizeof( line ) / sizeof( char ), fp ) ) {
		return true;
	}
	
	return false;
}

void advance() {
	/* 設計仕様と異なる */
	/* この実装では, 空白行, コメントアウト部分の削除も含めて行う */
	char * cp;
	char tmp_str[256];
	int i = 0, length = strlen( line ), type = 0;
	strcpy( tmp_str, line );

	if ( tmp_str[0] == '\r' ) {
		// 空白行の場合
		tmp_str[0] = '\0';
		fprintf( stdout, "blank line\n" );
		strcpy( cmd, tmp_str );
	} else { 
		if ( ( cp = strstr( tmp_str, "//" ) ) ) {
			// コメントアウト以降の文字列は不要のため，NULL文字に置換
			// 文字列のコマンド行最後とする
			*cp = '\0';
		}

		// のちの処理を改行コードを意識せずに行うことができるよう
		// 改行コードをNULL終端文字に置き換える
		for ( int i = strlen( tmp_str ) ; i > 0 ; i-- ) {
			if ( tmp_str[i] == '\r' ) {
				tmp_str[i] = '\0';
			}
		}

		InitCommand();
		strcpy( current_cmd, tmp_str );
		if ( ( cp = strtok( current_cmd, " " ) ) ) {
			// commandを格納
			strcpy( cmd, cp );
		}
		
		if ( ( cp = strtok( NULL, " " ) ) ) {
			// arg1を格納
			strcpy( argstr1, cp );
		}

		if ( ( cp = strtok( NULL, " " ) ) ) {
			// arg2を格納
			strcpy( argstr2, cp );
		}
	}
}

int commandType() {
	if ( strcmp( cmd, "add" ) == 0 || strcmp( cmd, "sub" ) == 0 || 
	     strcmp( cmd, "neg") == 0  || strcmp( cmd, "eq") == 0   || 
	     strcmp( cmd, "gt" ) == 0  || strcmp( cmd, "lt" ) == 0  || 
	     strcmp( cmd, "and" ) == 0 || strcmp( cmd, "or" ) == 0  || 
	     strcmp( cmd, "not" ) == 0 ) {
		return C_ARITHMETIC;
	} else if ( strcmp( cmd, "push" ) == 0 ) {
		return C_PUSH;
	} else if ( strcmp( cmd, "pop" ) == 0 ) {
		return C_POP;
	} else if ( strcmp( cmd, "function" ) == 0 ) {
		return C_FUNCTION;
	} else if ( strcmp( cmd, "call" ) == 0 ) {
		return C_CALL;
	} else if ( strcmp( cmd, "return" ) == 0 ) {
		return C_RETURN;
	} else if ( strcmp( cmd, "label" ) == 0 ) {
		return C_LABEL;
	} else if ( strcmp( cmd, "goto" ) == 0 ) {
		return C_GOTO;
	} else if ( strcmp( cmd, "if-goto" ) == 0 ) {
		return C_IF;
	}
	return -1;	
}

char * arg1() {
	char * cp;
	char tmp_str[256];
/*
	cp = strtok( current_cmd, " " );
	strcpy( argstr1, cp );
*/	
	return argstr1;
}

int arg2() {
	char * cp;
	int tmp_arg2;
	
/*	cp = strtok( current_cmd, " " );
	strcpy( argstr2, cp );
*/
	tmp_arg2 = atoi( argstr2 );
	
	return tmp_arg2;
}

void InitAll() {
	filename[0] = '\0';
	line[0] = '\0';
	current_cmd[0] = '\0';

	InitCommand();
}

void InitCommand() {
	cmd[0] = '\0';
	argstr1[0] = '\0';
	argstr2[0] = '\0';
}

void PrintAscii( char ** str ) {
	int length = strlen( line );

	fprintf( stdout, "NO:%3d ", length );
	for ( int i = 0 ; i < length ; i++ ) {
		fprintf( stdout, "[%2d]", line[i] );
	}
	fprintf( stdout, "\n" );
}

void makeOutputFilename() {
	outputfilename[0] = 0;
	char * cp;
	char tmpfname[256];
	strcpy( tmpfname, filename );

	cp = strstr( tmpfname, ".vm" );
	*cp = '\0';
	strcpy( fnameex2, tmpfname );
	snprintf( outputfilename, 256, "%s.asm", tmpfname );
	fprintf( stdout, "%s\n", outputfilename );
}
