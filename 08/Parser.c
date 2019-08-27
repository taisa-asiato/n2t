#include "define.h"

// 入力ファイルへのファイルポインタ
FILE * fp;
// 入力ファイルのファイルネーム
char inputfilename[1024];
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

unsigned int  eqnum = 0;
unsigned int  ltnum = 0;
unsigned int  gtnum = 0;
unsigned int retval = 0;
unsigned int framenum = 0;

int main( int argv, char ** argc ) {
	DIR * dirp;
	struct dirent * entp;
	int loopcount = 0;
	char tmpname[256];

	// 入力ディレクトリが無かった場合の動作
	if ( argv != 2 ) {
		fprintf( stdout, "[Usage]./VMTranslator dirname\n" );
		return 0;
	}

	if ( ( dirp = opendir( argc[1] ) ) ) {
		// 入力ディレクトリが存在する
		fprintf( stdout, "Open Directory\n" );
		while ( ( entp = readdir( dirp ) ) != NULL ) {
			strcpy( tmpname, entp->d_name );
			if ( strstr( tmpname, ".vm" ) != NULL ) {
				makeFileName( argc[1], entp->d_name );
				fprintf( stdout, "=== >>> %s <<< ===\n", entp->d_name );
				if ( loopcount == 0 ) {
					makeOutputFilename( argc[1] );
				}
				InitAll();	
				fprintf( stdout, "%s\n", inputfilename );
				fprintf( stdout, "[ParserMain Start]\n" );
				VMTransMain( loopcount );
				loopcount++;
			}
		}
	} else {
		fprintf( stdout, "Directory not found\n" );
	}
	return 0;	
}

// TODO: 関数名をParseMainから変更する必要あり
//     : 関数内部での処理がparse以外のことも行なっているため
//     : PasereMain -> VMTransMainにする
void VMTransMain( int count ) {
	int type = 0, arg2c = 0;
	fp = fopen( inputfilename, "r" ); 

	setFileName( count );

	if ( count == 0 ) {
		// 初回だけブートストラップコードを挿入する
		// fprintf( outputfp, "//This is BootStrap Code\n" );
		// fprintf( stdout, "//This is BootStrap Code\n" );
		writeInit();
	}

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
		// fprintf( stdout, "blank line\n" );
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
	return argstr1;
}

int arg2() {
	char * cp;
	int tmp_arg2;
	
	tmp_arg2 = atoi( argstr2 );
	
	return tmp_arg2;
}

void InitAll() {
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

void makeOutputFilename( char * str ) {
	outputfilename[0] = 0;
	char * cp;
	char tmpfname[256];
	char pre_str[256];

	strcpy( pre_str, str );
	cp = strtok( pre_str, "/" );
	while ( cp != NULL ) {
		strcpy( tmpfname, cp );
		cp = strtok( NULL, "/" );
		// fprintf( stdout, "%s\n", cp );
	}
	snprintf( outputfilename, 256, "%s%s.asm", str, tmpfname );
	// fprintf( stdout, "%s\n", outputfilename );
}

void makeFileName( char * d_name, char * f_name ) {
	sprintf( inputfilename, "%s%s", d_name, f_name );
	// fprintf( stdout, "%s\n", inputfilename  );
}
