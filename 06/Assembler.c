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
char bitcomp[18][7];
// jumpのbit列を表す
char bitjump[8][4];
// アドレスデータ
static const char binstr[65536][17];

int main() {
	int type = 0, length = 0;
	char cpystr[256];
	//strcpy ( fname, "./pong/Pong.asm" );
	strcpy ( fname, "./add/Add.asm" );
	// 入力ファイルを開く
	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	DestDataInit();
	JumpDataInit();
	CompDataInit();
	// PrintData();

	while ( hasMoreCommands() ) {
		strcpy( cpystr, current_cmd );
		length = strlen( current_cmd );
		cpystr[length-2] = '\0';
		type = parserMain();
		//fprintf( stdout, "%d\t%s\n", type, current_cmd );
		
		if ( type == A_COMMAND || type == C_COMMAND ) {
			//fprintf( stdout, "%s\t\t\t:", cpystr );
			if ( type == A_COMMAND ) {
				fprintf( stdout, "0%s\n", binstr[atoi( retsymbol )] );
			} else if ( type == C_COMMAND ) {
				if ( CodeAorM() == typeM ) {
					fprintf( stdout, "1111%s%s%s\n", CodeComp( retcomp ), 
						CodeDest( retdest ), CodeJump( retjump ) );
				} else {
					fprintf( stdout, "1110%s%s%s\n", CodeComp( retcomp ),
						CodeDest( retdest ), CodeJump( retjump ) );
				}
			}
		} else {
			// fprintf( stdout, "%s\n", cpystr );
		}
		
	}
	fclose( fp );
	return 0;
}

char *  IntegerToBinaryString( char integer[256] ) {
	int before = atoi( integer );
	return binstr[before];	
}
