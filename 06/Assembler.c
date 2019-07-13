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
// char symboltable[0xFFFF+1][0xFF+1];
char specialsymbol[5][10];
symandaddress symboltable[0xFFFF+1];

// 登録シンボル数のカウンタ
int symbolcnt = 0;
int tableoffset = 23;
int ramoffset = 16;
int reg = 0;

int main( int argv, char ** args ) {

	strcpy ( fname, args[1] );
	DestDataInit();
	JumpDataInit();
	CompDataInit();
	SymbolTableInit();

	// シンボル登録用のループ
	FirstLoop();
	SecondLoop();


	return 0;
}

char *  IntegerToBinaryString( char integer[256] ) {
	int before = atoi( integer );
	return binstr[before];	
}

int FirstLoop() {
	char ** tp;
	int type = 0, tmp = 0;

	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	while ( hasMoreCommands() ) {
		type = parserMain();
		if ( type == L_COMMAND ) {
			if ( !contains( retsymbol ) ) {
				// FirstLoopではSymbolTableに登録されていない
				// シンボル情報を登録するのみ

				addEntry( retsymbol, symbolcnt-tmp );
				tmp++;
				reg++;
			}
		}
		if ( type > 0 ) { symbolcnt++; }
	}

	fclose( fp );
	return 1;
}

int SecondLoop() {
	int type = 0, length = 0, address = 0, line = 0;
	char cpystr[256];
	// symbolcnt = 16;

	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return 0;
	}

	while ( hasMoreCommands() ) {
	 	type = 0;
		type = parserMain();
		strcpy( cpystr, current_cmd );
		length = strlen( current_cmd );
		cpystr[length-1] = '\0';
		cpystr[length-2] = '\0';

		if ( type == A_COMMAND || type == C_COMMAND ) {
			if ( type == A_COMMAND ) {
				if ( IsString( retsymbol ) ) {
					if ( -1 == ( address = getAddress( retsymbol ) ) ) {
						for ( address = 0  ; HaveContents( address ) == true ; address++ ) { ; }
						// symbolcnt++;
						addEntry( retsymbol, ramoffset );
						address = ramoffset;
						ramoffset++;
						reg++;
					}
					fprintf( stdout, "0%s\n", binstr[address] );
				} else {
					fprintf( stdout, "0%s\n", binstr[atoi( retsymbol )] );
				}
			} else if ( type == C_COMMAND ) {
				if ( CodeAorM() == typeM ) {
					fprintf( stdout, "1111%s%s%s\n", 
						CodeComp( retcomp ), CodeDest( retdest ), CodeJump( retjump ) );
				} else {
					fprintf( stdout, "1110%s%s%s\n", 
						CodeComp( retcomp ), CodeDest( retdest ), CodeJump( retjump ) );
				}
			} 
		} else {
			// fprintf( stdout, "E_COMMAND\n"  );
		}
		line++;
		
	}
	fclose( fp );
	return 0;
}

bool IsString( char * strsymbol ) {
	int strsize = strlen( strsymbol );

	for ( int i = 0 ; i < strsize ; i++ ) {
		if ( isdigit( strsymbol[i] ) ) {
			return false;
		} else {
			break;
		}
	}
	return true;
}

void PrintStrASCII() {
	for ( int i = 0 ; i < 10 ; i++ ) {
		fprintf( stdout, "0x%x:", current_cmd[i] );
	}
	fprintf( stdout, "\n" );
}

bool HaveContents( int address ) {
	if ( symboltable[address].symbolname[0] == '\0' || symboltable[address].symboladdress == -1 ) {
		return false;
	}
	return true;
}
