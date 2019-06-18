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
// compからの返り値
char retdest[4];
// symbolからの返り値
char retsymbol[256];
// compからの返り値
char retcomp[10];

void parserMain() {

	int cmdtype = 0;
	int length = 0;
	char symbolstring[256];
	char symbolstring2[256];

	strcpy( current_cmd, "" );
	strcpy ( fname, "./pong/Pong.asm" );
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
		length = strlen( current_cmd );
		if ( current_cmd[length-1] == '\n' ) {
			current_cmd[length-1] = '\0';
		}
		
		cmdtype = commandType();

		if ( cmdtype == A_COMMAND || cmdtype == L_COMMAND ) {
			// fprintf( stdout, "A or E: %s\n", current_cmd );
			strncpy( symbolstring, symbol(), 256 );
		} else if ( cmdtype == C_COMMAND ) {
			strncpy( symbolstring, dest(), 256 );
			strncpy( symbolstring2, comp(), 256 );
		} 

		if ( cmdtype == A_COMMAND ) {
			fprintf( stdout, "[A_COMMAND]: %s\n", symbolstring );
		} else if ( cmdtype == C_COMMAND ) {
			fprintf( stdout, "[C_COMMAND]: %s = %s\n", symbolstring, symbolstring2 );
		} else if ( cmdtype == L_COMMAND ){
			fprintf( stdout, "[L_COMMAND]: %s\n", symbolstring );
		} else if ( cmdtype == E_COMMENT ) {
			fprintf( stdout, "[E_COMMENT]:%s\n", current_cmd );
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

	if ( 1 == strlen( current_cmd ) || current_cmd[0] == '\0' ) {
		// 空白行
		return E_BLANK;
	} else if ( current_cmd[0] == '/' && current_cmd[1] == '/' ) { 
		// コメントアウトの行
		return E_COMMENT;
	} else if ( current_cmd[0] == '@' ) {
		// コマンド行の先頭が@の場合はA
		return A_COMMAND;
	} else if ( current_cmd[0] == '(' && current_cmd[strlen(current_cmd)-2] == ')' ) { 
		// (Xxx)はL
		return L_COMMAND;
	} else {
		if ( ( splitstr = strtok( current_cmd, ";" ) ) || 
		     ( splitstr = strtok( current_cmd, "=" ) ) ) {
			// コマンド行がジャンプ命令, 代入命令の場合はC
			return C_COMMAND;
		}
	}
	return E_CMDERR;
}

char * symbol() {
	int i = 0, j = 0;

	if ( current_cmd[0] == '@' ) {
		// A_COMMANDの場合
		int length = strlen( current_cmd );
		for ( i = 1, j = 0 ; i < length ; i++, j++ ) {
			retsymbol[j] = current_cmd[i];
		}
	} else {
		// L_COMMANDの場合
		int length = strlen( current_cmd );
		for ( i = 1, j = 0 ; i < length-2 ; i++, j++ ) {
			retsymbol[j] = current_cmd[i];
		}
	}
	retsymbol[j] = '\0';

	return retsymbol;
}

char * dest() {
	char str[256];
	char * strpt;
	int i = 0;

	strncpy( str, current_cmd, sizeof( str ) / sizeof( char ) );
	strpt = strtok( str, "=" );
	for ( i = 0 ; i < strlen( strpt ) ; i++ ) {
		retdest[i] = *( strpt + i );
	}
	retdest[i] = '\0';
	// fprintf( stdout, "In:%s, copied word is %s, length is %lu\n", __func__, retdest, strlen( strpt) );

	return retdest;
}

char * comp() {
	char strpt[256];
	int i = 0, len = strlen( current_cmd ), j = 0 ;
	
	strncpy( strpt, current_cmd, sizeof( strpt ) / sizeof( char ) );

	while ( strpt[i] != '=' ) {
		i++;
	}

	i++; // whileでのiの位置は=の位置を指しているため，位置を1進める
	for ( j = 0 ; strpt[i] != '\0' ; j++,i++ ) {
		retcomp[j] = strpt[i];
	}
	retcomp[j] = '\0';
	// fprintf( stdout, "In:%s, copied word is %s, length is %lu\n", 
	//		__func__, retcomp, strlen( strpt) );
	return retcomp;
}
