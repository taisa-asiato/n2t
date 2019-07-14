#include "define.h"

// 入力ファイルへのファイルポインタ
FILE * fp;
// 入力ファイルのファイルネーム
char filename[256];
// 入力ストリーム
char line[256];
// 現在のコマンド
char current_cmd[256];

int main( int argv, char ** argc ) {
	Init();
	strcpy( filename, argc[1] );
	fprintf( stdout, "%s\n", filename );
	ParseMain();
	return 0;	
}

void ParseMain() {
	fp = fopen( filename, "r" ); 

	while ( hasMoreCommands() ) {
		// PrintAscii( line );
		advance();
		fprintf( stdout, "%s\n", current_cmd );	
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
	int i = 0, length = strlen( line );
	strcpy( tmp_str, line );

	if ( tmp_str[0] == '\r' ) {
		// 空白行の場合
		tmp_str[0] = '\0';
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
	}
	strcpy( current_cmd, tmp_str );
}

void Init() {
	filename[0] = '\0';
	line[0] = '\0';
	current_cmd[0] = '\0';
}

void PrintAscii( char ** str ) {
	int length = strlen( line );

	fprintf( stdout, "NO:%3d ", length );
	for ( int i = 0 ; i < length ; i++ ) {
		fprintf( stdout, "[%2d]", line[i] );
	}
	fprintf( stdout, "\n" );
}
