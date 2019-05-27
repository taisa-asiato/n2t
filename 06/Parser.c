#include "define.h"

#define A_COMMAND 1
#define C_COMMAND 2
#define L_COMMAND 3
#define E_CMDERR -1
#define E_COMMENT -2
#define E_BLANK -3

// 現在のコマンドを保持する
char current_cmd[256];
// 一つ前のコマンドを保持する変数
char previous_cmd[256];

void parserMain() {

	int cmdtype = 0;
	// current_cmdは初期状態はから
	strcpy( current_cmd, "" );

	strcpy ( fname, "./add/Add.asm" );
	// 入力ファイルを開く
	if ( ( fp = fopen( fname, "r" ) ) == NULL )  {
		fprintf( stdout, "file not found\n" );
		return;
	}

	while ( hasMoreCommands() ) {
		// 入力ファイルがまだコマンドが存在する場合
		strncpy( previous_cmd, current_cmd, sizeof( previous_cmd ) / sizeof( char ) );
		// 現在のコマンドに入力ストリームの入力を代入
		strncpy( current_cmd, str, sizeof( str ) / sizeof( char ) );
		cmdtype = commandType();
		if ( cmdtype == A_COMMAND ) {
			fprintf( stdout, "%s ---> : A_COMMAND\n", current_cmd );
		} else if ( cmdtype == C_COMMAND ) {
			fprintf( stdout, "%s ---> : C_COMMAND\n", current_cmd );
		} else if ( cmdtype == L_COMMAND ){
			fprintf( stdout, "%s ---> : L_COMMAND\n", current_cmd );
		} else if ( cmdtype == E_COMMENT ) {
			fprintf( stdout, "%s ---> : Comment out\n", current_cmd );
		} else if ( cmdtype == E_CMDERR ) {
			fprintf( stdout, "ERROR: Command type is not found\n" );
		} else if ( cmdtype == E_BLANK ) {
			fprintf( stdout, " ---> : Blank line\n" );
		}
	} 
	fclose ( fp );
	return;
}

bool hasMoreCommands ( ) {
	// ファイルから一行読み出す
	if ( fgets( str, sizeof( str ) / sizeof( char ), fp ) ) {
		// コマンドが他にある場合, trueを返す
		return true;
	} else {
		// コマンドが他に無い場合, falseを返す
		return false;
	}
}

int commandType() {
	char * splitstr;

	if ( current_cmd[0] == '/' && current_cmd[1] == '/' ) { 
		// コメントアウトの行
		// コード変換不要
		return E_COMMENT;
	} else if ( isspace( current_cmd ) ) {
		// 空白行
		return E_BLANK;
	} else if ( current_cmd[0] == '@' ) {
		// コマンド行の先頭が@の場合はA
		return A_COMMAND;
	} else {
		if ( splitstr = strtok( current_cmd, ";" ) ) {
			// コマンド行がジャンプ命令の場合はC
			return C_COMMAND;
		} else { 
			// 上記2つ以外のコマンドの場合はL
			return L_COMMAND;
		}
	}
	return E_CMDERR;
}
