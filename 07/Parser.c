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



int main( int argv, char ** argc ) {
	InitAll();
	strcpy( filename, argc[1] );
	fprintf( stdout, "%s\n", filename );
	ParseMain();
	return 0;	
}

void ParseMain() {
	int type = 0, arg2c = 0;
	fp = fopen( filename, "r" ); 

	while ( hasMoreCommands() ) {
		// PrintAscii( line );
		advance();
		type = commandType();

		fprintf( stdout, "%s", cmd );
		if ( type != C_RETURN ) {
			arg1();
			fprintf( stdout, " %s", argstr1 );
		}

		arg2c = 0;
		if ( type == C_PUSH || type == C_POP || type == C_FUNCTION || type == C_CALL ) {
			arg2c = arg2();
			fprintf( stdout, " %d", arg2c );
		}
		fprintf( stdout, "\n" );	
	}

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
		// strcpy( current_cmd, tmp_str );
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
	}
	return 1;	
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
