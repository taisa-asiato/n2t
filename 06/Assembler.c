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
// シンボル登録テーブル
char symboltable[0xFFFF+1][0xFF+1];
char specialsymbol[5][10];

// 登録シンボル数のカウンタ
int symbolcnt = 0;

int main() {
	strcpy ( fname, "./pong/Pong.asm" );
	// strcpy ( fname, "./add/Add.asm" );
	DestDataInit();
	JumpDataInit();
	CompDataInit();
	SymbolTableInit();

	// シンボル登録用のループ
	FirstLoop();
	// PrintTable();
	SecondLoop();

	return 0;
}

char *  IntegerToBinaryString( char integer[256] ) {
	int before = atoi( integer );
	return binstr[before];	
}

int FirstLoop() {
	char ** tp;
	int type = 0;

	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	while ( hasMoreCommands() ) {
		type = parserMain();
		// fprintf( stdout, "%s", current_cmd );
		if ( type > 0 ) { symbolcnt++; }
		if ( type == L_COMMAND ) {
			if ( !contains( retsymbol ) ) {
				// FirstLoopではSymbolTableに登録されていない
				// シンボル情報を登録するのみ
				// fprintf( stdout, "%s is not registered\n", retsymbol );
				addEntry( retsymbol, symbolcnt );
			}
		}
	}

	fclose( fp );
	return 1;
}

int SecondLoop() {
	int type = 0, length = 0, address = 0;
	char cpystr[256];

	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	while ( hasMoreCommands() ) {
	// 	strcpy( cpystr, current_cmd );
		length = strlen( current_cmd );
		cpystr[length-1] = '\0';
		cpystr[length-2] = '\0';
		cpystr[length-3] = '\0';
		type = parserMain();
		fprintf( stdout, "%s:", current_cmd );
		
		if ( type == A_COMMAND || type == C_COMMAND || type == L_COMMAND) {
			//fprintf( stdout, "%s\t\t\t:", cpystr );
			if ( type == A_COMMAND ) {
				if ( IsString( retsymbol ) ) {
					address = getAddress( retsymbol );
					fprintf( stdout, "%s [%4d]:0%s\n", retsymbol, address, binstr[address] );
				} else {
					fprintf( stdout, "%s 0%s\n", retsymbol, binstr[atoi( retsymbol )] );
				}
			} else if ( type == C_COMMAND ) {
				if ( CodeAorM() == typeM ) {
					fprintf( stdout, "1111%s%s%s\n", CodeComp( retcomp ), CodeDest( retdest ), CodeJump( retjump ) );
				} else {
					fprintf( stdout, "1110%s%s%s\n", CodeComp( retcomp ),CodeDest( retdest ), CodeJump( retjump ) );
				}
			//} else if ( type == L_COMMAND ) {
			//	address = getAddress( retsymbol );
			}
		} else {
			// fprintf( stdout, "%s\n", cpystr );
		}
		
	}
	fclose( fp );
	return 0;
}

bool IsString( char retsymbol[10] ) {
	int strsize = strlen( retsymbol );

	for ( int i = 0 ; i < strsize || retsymbol[i] != '\0' ; i++ ) {
		if ( isdigit( retsymbol[i] ) ) {
			return false;
		}
	}
	return true;
}
