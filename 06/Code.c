#include "define.h"

void DestDataInit() {
	strcpy( bitdest[0], "000" );
	strcpy( bitdest[1], "001" );
	strcpy( bitdest[2], "010" );
	strcpy( bitdest[3], "011" );
	strcpy( bitdest[4], "100" );
	strcpy( bitdest[5], "101" );
	strcpy( bitdest[6], "110" );
	strcpy( bitdest[7], "111" );

	for ( int i = 0 ; i < 8 ; i++ ) {
		fprintf( stdout, "%s\n", bitdest[i] );
	}
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

	for ( int i = 0 ; i < 8 ; i++ ) {
		fprintf( stdout, "%s\n", bitjump[i] );
	}
}

void CompuDataInit(){
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
}

char * CodeDest( char strcode[10] ) {
	return NULL;
}

char * CodeComp( char strcomp[10] ) {
	return NULL;
}

char * CodeJump( char strjump[10] ) {
	return NULL;	
}
