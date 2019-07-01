#include "define.h"

void FunctionPrint( char * str ) {
	fprintf( stdout, "%s\n", str );
}

void DestDataInit() {
	strcpy( /*null*/ bitdest[0], "000\0" );
	strcpy( /* M  */ bitdest[1], "001\0" );
	strcpy( /* D  */ bitdest[2], "010\0" );
	strcpy( /* MD */ bitdest[3], "011\0" );
	strcpy( /* A  */ bitdest[4], "100\0" );
	strcpy( /* AM */ bitdest[5], "101\0" );
	strcpy( /* AD */ bitdest[6], "110\0" );
	strcpy( /* AMD*/ bitdest[7], "111\0" );
	//FunctionPrint( __func__ );
//	for ( int i = 0 ; i < 8 ; i++ ) {
//		fprintf( stdout, "%s\n", bitdest[i] );
//	}
}

void JumpDataInit() {
	strcpy( /*null*/ bitjump[0], "000\0" );
	strcpy( /*JGT */ bitjump[1], "001\0" );
	strcpy( /*JEQ */ bitjump[2], "010\0" );
	strcpy( /*JGE */ bitjump[3], "011\0" );
	strcpy( /*JLT */ bitjump[4], "100\0" );
	strcpy( /*JNE */ bitjump[5], "101\0" );
	strcpy( /*JLE */ bitjump[6], "110\0" );
	strcpy( /*JMP */ bitjump[7], "111\0" );
	//FunctionPrint( __func__ );
//	for ( int i = 0 ; i < 8 ; i++ ) {
//		fprintf( stdout, "%s\n", bitjump[i] );
//	}
}

void CompDataInit(){
	strcpy( bitcomp[0], "101010\0" );
	strcpy( bitcomp[1], "111111\0" );
	strcpy( bitcomp[2], "111010\0" );
	strcpy( bitcomp[3], "001100\0" );
	strcpy( bitcomp[4], "110000\0" );
	strcpy( bitcomp[5], "001101\0" );
	strcpy( bitcomp[6], "110001\0" );
	strcpy( bitcomp[7], "001111\0" );
	strcpy( bitcomp[8], "110011\0" );
	strcpy( bitcomp[9], "011111\0" );
	strcpy( bitcomp[10], "110111\0" );
	strcpy( bitcomp[11], "001110\0" );
	strcpy( bitcomp[12], "110010\0" );
	strcpy( bitcomp[13], "000010\0" );
	strcpy( bitcomp[14], "010011\0" );
	strcpy( bitcomp[15], "000111\0" );
	strcpy( bitcomp[16], "000000\0" );
	strcpy( bitcomp[17], "010101\0" );
	//FunctionPrint( __func__ );
}

char * CodeDest( char strcode[10] ) {
	if ( strcmp( strcode, "" ) == 0 || strlen( strcode ) == 0 || strcode[0] == '\0' || strcode[0] == 48 ) {
		return bitdest[0];
	} else if ( strcmp( strcode, "M\0" ) == 0 ) {
		return bitdest[1];
	} else if ( strcmp( strcode, "D\0" ) == 0 ) {
		return bitdest[2];
	} else if ( strcmp( strcode, "MD\0" ) == 0 ) {
		return bitdest[3];
	} else if ( strcmp( strcode, "A\0" ) == 0 ) {
		return bitdest[4];
	} else if ( strcmp( strcode, "AM\0" ) == 0 ) {
		return bitdest[5];
	} else if ( strcmp( strcode, "AD\0" ) == 0 ) {
		return bitdest[6];
	} else if ( strcmp( strcode, "AMD\0" ) == 0 ) {
		return bitdest[7];
	}
	// fprintf( stdout, "%s\n", __func__  );
	for ( int i = 0 ; i < strlen( strcode ) ; i++ ) {
		fprintf( stdout, "%d\n", strcode[i] );
	}
	return NULL;
}

char * CodeComp( char strcode[10] ) {
	if ( strcmp( strcode, "" ) == 0 || strlen( strcode ) == 0 || strcode[0] == '\0' || strcode[0] == 48 ) {
		return bitcomp[0];
	} else if ( strcmp( strcode, "1" ) == 0 ) {
		return bitcomp[1];
	} else if ( strcmp( strcode, "-1" ) == 0 ) {
		return bitcomp[2];
	} else if ( strcmp( strcode, "D" ) == 0 ) {
		return bitcomp[3];
	} else if ( strcmp( strcode, "A" ) == 0 || strcmp( strcode, "M" ) == 0 ) {
		return bitcomp[4];
	} else if ( strcmp( strcode, "!D" ) == 0 ) {
		return bitcomp[5];
	} else if ( strcmp( strcode, "!A" ) == 0 || strcmp( strcode, "!M" ) == 0 ) {
		return bitcomp[6];
	} else if ( strcmp( strcode, "-D" ) == 0 ) {
		return bitcomp[7];
	} else if ( strcmp( strcode, "-A" ) == 0 || strcmp( strcode, "-M" ) == 0 ) {
		return bitcomp[8];
	} else if ( strcmp( strcode, "D+1" ) == 0 ) {
		return bitcomp[9];
	} else if ( strcmp( strcode, "A+1" ) == 0 || strcmp( strcode, "M+1" ) == 0 ) {
		return bitcomp[10];
	} else if ( strcmp( strcode, "D-1" ) == 0 ) {
		return bitcomp[11];
	} else if ( strcmp( strcode, "A-1" ) == 0 || strcmp( strcode, "M-1" ) == 0 ) {
		return bitcomp[12];
	} else if ( strcmp( strcode, "D+A" ) == 0 || strcmp( strcode, "D+M" ) == 0 ) {
		return bitcomp[13];
	} else if ( strcmp( strcode, "D-A" ) == 0 || strcmp( strcode, "D-M" ) == 0 ) {
		return bitcomp[14];
	} else if ( strcmp( strcode, "A-D" ) == 0 || strcmp( strcode, "M-D" ) == 0 ) {
		return bitcomp[15];
	} else if ( strcmp( strcode, "D&A" ) == 0 || strcmp( strcode, "D&M" ) == 0 ) {
		return bitcomp[16];
	} else if ( strcmp( strcode, "D|A" ) == 0 || strcmp( strcode, "D|M" ) == 0 ) {
		return bitcomp[17];
	} 
	// fprintf( stdout, "%s\n", __func__  );
	fprintf( stdout, "%d 0x%x\n %c is word", strlen( strcode ), strcode[0], strcode[0]  );
	for ( int i = 0 ; i < strlen( strcode ) ; i++ ) {
		fprintf( stdout, "%d\n", strcode[i] );
	}
	return NULL;
}

char * CodeJump( char strcode[10] ) {
	if ( strcmp( strcode, "" ) == 0 || strlen( strcode ) == 0 || strcode[0] == '\0' || strcode[0] == 48 ) {
		return bitjump[0];
	} else if ( strcmp( strcode, "JGT" ) == 0 ) {
		return bitjump[1];
	} else if ( strcmp( strcode, "JEQ" ) == 0 ) {
		return bitjump[2];
	} else if ( strcmp( strcode, "JGE" ) == 0 ) {
		return bitjump[3];
	} else if ( strcmp( strcode, "JLT" ) == 0 ) {
		return bitjump[4];
	} else if ( strcmp( strcode, "JNE" ) == 0 ) {
		return bitjump[5];
	} else if ( strcmp( strcode, "JLE" ) == 0 ) {
		return bitjump[6];
	} else if ( strcmp( strcode, "JMP" ) == 0 ) {
		return bitjump[7];
	}
	// fprintf( stdout, "%s\n", __func__  );
	return NULL;
}

int CodeAorM() {
	if ( strstr( retcomp, "M" ) ) {
		return typeM;
	} else {
		return typeA;
	}
}

void PrintData() {
	for ( int i = 0 ; i < 8 ; i++ ) {
		fprintf( stdout, "%s\n", bitdest[i] );
	}

	for ( int i = 0 ; i < 18 ; i++ ) {
		fprintf( stdout, "%s\n", bitcomp[i] );
	}

	for ( int i = 0 ; i < 8 ; i++ ) {
		fprintf( stdout, "%s\n", bitjump[i] );
	}
}
