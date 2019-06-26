#include "define.h"

void FunctionPrint( char * str ) {
	fprintf( stdout, "%s\n", str );
}

void DestDataInit() {
	strcpy( bitdest[0], "000" );
	strcpy( bitdest[1], "001" );
	strcpy( bitdest[2], "010" );
	strcpy( bitdest[3], "011" );
	strcpy( bitdest[4], "100" );
	strcpy( bitdest[5], "101" );
	strcpy( bitdest[6], "110" );
	strcpy( bitdest[7], "111" );
	FunctionPrint( __func__ );
//	for ( int i = 0 ; i < 8 ; i++ ) {
//		fprintf( stdout, "%s\n", bitdest[i] );
//	}
}

void JumpDataInit() {
	strcpy( bitjump[0], "000" );
	strcpy( bitjump[1], "001" );
	strcpy( bitjump[2], "010" );
	strcpy( bitjump[3], "011" );
	strcpy( bitjump[4], "100" );
	strcpy( bitjump[5], "101" );
	strcpy( bitjump[6], "110" );
	strcpy( bitjump[7], "111" );
	FunctionPrint( __func__ );
//	for ( int i = 0 ; i < 8 ; i++ ) {
//		fprintf( stdout, "%s\n", bitjump[i] );
//	}
}

void CompDataInit(){
	strcpy( bitcomp[0], "101010" );
	strcpy( bitcomp[1], "111111" );
	strcpy( bitcomp[2], "111010" );
	strcpy( bitcomp[3], "001100" );
	strcpy( bitcomp[4], "110000" );
	strcpy( bitcomp[5], "001101" );
	strcpy( bitcomp[6], "110001" );
	strcpy( bitcomp[7], "001111" );
	strcpy( bitcomp[8], "110011" );
	strcpy( bitcomp[9], "011111" );
	strcpy( bitcomp[10], "110111" );
	strcpy( bitcomp[11], "001110" );
	strcpy( bitcomp[12], "110010" );
	strcpy( bitcomp[13], "000010" );
	strcpy( bitcomp[14], "010011" );
	strcpy( bitcomp[15], "000111" );
	strcpy( bitcomp[16], "000000" );
	strcpy( bitcomp[17], "010101" );
	FunctionPrint( __func__ );
}

char * CodeDest( char strcode[10] ) {
	if ( strcmp( strcode, "" ) ) {
		return bitdest[0];
	} else if ( strcmp( strcode, "M" ) ) {
		return bitdest[1];
	} else if ( strcmp( strcode, "D" ) ) {
		return bitdest[2];
	} else if ( strcmp( strcode, "MD" ) ) {
		return bitdest[3];
	} else if ( strcmp( strcode, "A" ) ) {
		return bitdest[4];
	} else if ( strcmp( strcode, "AM" ) ) {
		return bitdest[5];
	} else if ( strcmp( strcode, "AD" ) ) {
		return bitdest[6];
	} else if ( strcmp( strcode, "AMD" ) ) {
		return bitdest[7];
	}
	return NULL;
}

char * CodeComp( char strcode[10] ) {
	if ( strcmp( strcode, "0" ) ) {
		return bitcomp[0];
	} else if ( strcmp( strcode, "1" ) ) {
		return bitcomp[1];
	} else if ( strcmp( strcode, "-1" ) ) {
		return bitcomp[2];
	} else if ( strcmp( strcode, "D" ) ) {
		return bitcomp[3];
	} else if ( strcmp( strcode, "A" ) || strcmp( strcode, "M" ) ) {
		return bitcomp[4];
	} else if ( strcmp( strcode, "!D" ) ) {
		return bitcomp[5];
	} else if ( strcmp( strcode, "!A" ) || strcmp( strcode, "!M" ) ) {
		return bitcomp[6];
	} else if ( strcmp( strcode, "-D" ) ) {
		return bitcomp[7];
	} else if ( strcmp( strcode, "-A" ) || strcmp( strcode, "-M" ) ) {
		return bitcomp[8];
	} else if ( strcmp( strcode, "D+1" ) ) {
		return bitcomp[9];
	} else if ( strcmp( strcode, "A+1" ) || strcmp( strcode, "M+1" ) ) {
		return bitcomp[10];
	} else if ( strcmp( strcode, "D-1" ) ) {
		return bitcomp[11];
	} else if ( strcmp( strcode, "A-1" ) || strcmp( strcode, "M-1" ) ) {
		return bitcomp[12];
	} else if ( strcmp( strcode, "D+A" ) || strcmp( strcode, "D+M" ) ) {
		return bitcomp[13];
	} else if ( strcmp( strcode, "D-A" ) || strcmp( strcode, "D-M" ) ) {
		return bitcomp[14];
	} else if ( strcmp( strcode, "A-D" ) || strcmp( strcode, "M-D" ) ) {
		return bitcomp[15];
	} else if ( strcmp( strcode, "D&A" ) || strcmp( strcode, "D&M" ) ) {
		return bitcomp[16];
	} else if ( strcmp( strcode, "D|A" ) || strcmp( strcode, "D|M" ) ) {
		return bitcomp[17];
	} 
	return NULL;
}

char * CodeJump( char strcode[10] ) {
	if ( strcmp( strcode, "" ) ) {
		return bitjump[0];
	} else if ( strcmp( strcode, "JGT" ) ) {
		return bitjump[1];
	} else if ( strcmp( strcode, "JEQ" ) ) {
		return bitjump[2];
	} else if ( strcmp( strcode, "JGE" ) ) {
		return bitjump[3];
	} else if ( strcmp( strcode, "JLT" ) ) {
		return bitjump[4];
	} else if ( strcmp( strcode, "JNE" ) ) {
		return bitjump[5];
	} else if ( strcmp( strcode, "JLE" ) ) {
		return bitjump[6];
	} else if ( strcmp( strcode, "JMP" ) ) {
		return bitjump[7];
	}
	return NULL;
}
