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

char bitdest[8][4];
// compのbit列を表す
char bitcomp[18][6];
// jumpのbit列を表す
char bitjump[8][4];

int main() {
	int type = 0;
	strcpy ( fname, "./pong/Pong.asm" );
	// strcpy ( fname, "./add/Add.asm" );
	// 入力ファイルを開く
	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	DestDataInit();
	JumpDataInit();
	CompDataInit();

	while ( hasMoreCommands() ) {
		fprintf( stdout, "####>> %s", str );
		type = parserMain();
		fprintf( stdout, "%s%s%s\n", 
			CodeDest( retdest ), CodeComp( retcomp ), CodeJump( retjump ) );
	}
	fclose( fp );
		
	return 0;
}
