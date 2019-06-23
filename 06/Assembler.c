#include "define.h"

// 入力ファイルへのファイルポインタ
FILE * fp;
// 入力ファイルの名前
char fname[256];
// 入力ファイルの中身（1行）
char str[256];
// symbolからの返り値
char retsymbol[256] = { 0 };
// destの返り値 
char retdest[10];
// compの返り値
char retcomp[10];
// jumpの返り値
char retjump[10];
// 現在のコマンドを保持する
char current_cmd[256] = { 0 };

int main() {
	int type = 0;
	strcpy ( fname, "./pong/Pong.asm" );
	// strcpy ( fname, "./add/Add.asm" );
	// 入力ファイルを開く
	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	while ( hasMoreCommands() ) {
		fprintf( stdout, "####>> %s", str );
		type = parserMain();
		/*if ( type == A_COMMAND ) {
			fprintf( stdout, "%s\n", retsymbol );	
		} else if ( type == C_COMMAND ) {
			fprintf( stdout, "%s %s %s\n", retdest, retcomp, retjump );		
		} else if ( type == L_COMMAND ) {
			fprintf( stdout, "%s\n", retsymbol );	
		} else {
			fprintf( stdout, "error\n"  );
		}*/
		
	}
	fclose( fp );
		
	return 0;
}
