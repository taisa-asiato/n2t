#include "define.h"

// 一つ前のコマンドを保持する変数
char previous_cmd[256] = { 0 };

int parserMain() {

	int cmdtype = 0;
	int length = 0;
	char symbolstring[256];
	char symbolstring2[256];
	char symbolstring3[256];

	current_cmd[0] = '\0'; 
	symbolstring[0] = '\0', symbolstring2[0] = '\0', symbolstring3[0] = '\0';
	retdest[0] = '\0', retcomp[0] = '\0', retjump[0] = '\0';

	// 現在のコマンドに入力ストリームの入力を代入
	strcpy( current_cmd, str );
	length = strlen( current_cmd );

	/*
	fprintf( stdout, "len = %2d\n", length );
	for ( int i = 0 ; i < length ; i++ ) {
		fprintf( stdout, "%2d:%2d\n", i, current_cmd[i] );
	}
	*/

	DelCommentOut();
	DelSpace();
	cmdtype = commandType();

	if ( cmdtype == A_COMMAND || cmdtype == L_COMMAND ) {
		// fprintf( stdout, "A or E: %s\n", current_cmd );
		symbol();
	} else if ( cmdtype == C_COMMAND ) {
		// C_COMMANDの場合は, dest, comp, jump各関数を実行する
		dest(); comp(); jump();
	} 

/*	
	if ( cmdtype == A_COMMAND ) {
		fprintf( stdout, " [A_COMMAND]: %s\n", retsymbol );
	} else if ( cmdtype == C_COMMAND ) {
		fprintf( stdout, " [C_COMMAND]: %s=%s;%s\n", retdest, retcomp, retjump );
	} else if ( cmdtype == L_COMMAND ){
		fprintf( stdout, " [L_COMMAND]: %s\n", retsymbol );
	} else if ( cmdtype == E_COMMENT ) {
		fprintf( stdout, " [E_COMMENT]:%s\n", current_cmd );
	} else if ( cmdtype == E_CMDERR ) {
		fprintf( stdout, " ERROR: Command type is not found\n" );
	} else if ( cmdtype == E_BLANK ) {
		fprintf( stdout, " ---> : Blank line\n" );
	}
*/
	return cmdtype;
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

	if ( 0 == strlen( current_cmd ) || current_cmd[0] == '\0' ) {
		// 空白行
		return E_BLANK;
	} else if ( current_cmd[0] == '/' && current_cmd[1] == '/' ) { 
		// コメントアウトの行
		return E_COMMENT;
	} else if ( current_cmd[0] == '@' ) {
		// コマンド行の先頭が@の場合はA
		return A_COMMAND;
	} else if ( current_cmd[0] == '(' ) { 
		// (Xxx)はL
		return L_COMMAND;
	} else {
		if ( ( splitstr = strstr( current_cmd, ";" ) ) || 
		     ( splitstr = strstr( current_cmd, "=" ) ) ) {
			// コマンド行がジャンプ命令, 代入命令の場合はC
			return C_COMMAND;
		}
	}
	return E_CMDERR;
}

void DelCommentOut() {
	char * strpt;
	if ( ( strpt = strstr( current_cmd, "//" ) ) ) {
		// コメントアウトを含む行
		*strpt = '\r';
	}
}

void DelSpace() {
	char * strpt;
	int length = strlen( current_cmd );
	int i = 0;

	// strpt = current_cmd;
	while ( isspace( current_cmd[i] ) ) {
		for ( i = 0 ; i < length ; i++ ) {
			current_cmd[i] = current_cmd[i+1];
		}
		length--;
		i = 0;
	}
}

char * symbol() {
	int i = 0, j = 0;

	if ( current_cmd[0] == '@' ) {
		// A_COMMANDの場合
		for ( i = 1, j = 0 ; current_cmd[i] != '\r' ; i++, j++ ) {
			retsymbol[j] = current_cmd[i];
		}
	} else {
		// L_COMMANDの場合
		for ( i = 1, j = 0 ; current_cmd[i] != ')' ; i++, j++ ) {
			// current_cmdから左右の()を除いた文字だけ格納
			retsymbol[j] = current_cmd[i];
		}
	}
	retsymbol[j] = '\0';
	return retsymbol;
}

char * dest() {
	char * strpt;
	int i = 0;

	if ( ( strpt = strstr( current_cmd, "=" ) ) ) { 
		strpt = current_cmd;
		for ( i = 0 ; *strpt != '=' ; i++, strpt++ ) {
			retdest[i] = *strpt;
		}
		retdest[i] = '\0';
	} else {
	//	fprintf( stdout, "Not found =\n" );
		retdest[0] = '\0';
	}
	// PrintFunctionName( __func__ );	
	return retdest;
}

char * comp() {
	// char strpt[256];
	char * strpt;
	char * pt;
	int j = 0 ;

	if ( ( strpt = strstr( current_cmd, "=" ) ) ) {
		strpt++;
		if ( ( pt = strstr( current_cmd, ";" ) ) ) {
			for ( j = 0 ; *strpt != ';' ; j++, strpt++ ) {
				retcomp[j] = *strpt;
			}
		} else { 
			for ( j = 0 ; *strpt != '\r' ; j++, strpt++ ) {
				retcomp[j] = *strpt;
			}
		}
		retcomp[j] = '\0';
	} else 	if ( ( strpt = strstr( current_cmd, ";" ) ) ) { 
		strpt = current_cmd;
		for ( j = 0 ; *strpt != ';' ; j++, strpt++ ) {
			retcomp[j] = *strpt;
		}
		retcomp[j] = '\0';
	} else {
	//	fprintf( stdout, "Not found =\n" );
		retcomp[0] = '\0';
	}

	// PrintFunctionName( __func__ );	
	return retcomp;
}

char * jump() {
	char * strpt;
	int j = 0;
	
	if ( ( strpt = strstr( current_cmd, ";" ) ) ) {
		strpt++;
		for ( j = 0 ; *strpt != '\r' ; strpt++, j++ ) {
			retjump[j] = *strpt;
		}
		retjump[j] = '\0';
	} else {
	//	fprintf( stdout, "Not found ;\n" );
		retjump[0] = '\0';
	}
	
	// PrintFunctionName( __func__ );	
	return retjump;
}

void PrintFunctionName( char * funcname ) {
	fprintf( stdout, "%s\n", funcname );
}
