#include "define.h"

void SymbolTableInit() {
	char cpystr[4];

	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		strcpy( symboltable[i], "\0" );
	}
	
	for ( int i = 0 ; i < 16 ; i++ ) {
		sprintf( cpystr, "R%d", i );
		strcpy( symboltable[i], cpystr );
	}

	strcpy( symboltable[0x4000], "SCREEN" );
	strcpy( symboltable[0x6000], "KBD" );

	/* Special Symbolのテーブルを別途作成 */
	strcpy( specialsymbol[0x0000], "SP\0" );
	strcpy( specialsymbol[0x0001], "LCL\0" );
	strcpy( specialsymbol[0x0002], "ARG\0" );
	strcpy( specialsymbol[0x0003], "THIS\0" );
	strcpy( specialsymbol[0x0004], "THAT\0" );
}

void addEntry( char * symbol, int address ) {
	strcpy( symboltable[address], symbol );
}

bool contains( char * symbol ) {
	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		if ( strcmp( symboltable[i], symbol ) == 0 ) {
			return true;
		}
	}

	for ( int i = 0 ; i < 5 ; i++ ) {
		if ( strcmp( specialsymbol[i], symbol ) == 0 ) {
			return true;
		}
	}

	return false;
}

int getAddress( char * symbol ) {
	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		if ( strcmp( symboltable[i], symbol ) == 0 ) {
			return i;
		}
	}
	for ( int i = 0 ; i < 5 ; i++ ) {
		if ( strcmp( specialsymbol[i], symbol ) == 0 ) {
			return i;
		}
	}
	return -1;
}

void PrintTable() {
	fprintf( stdout, "%s\n",  symboltable[0x0000] );
	fprintf( stdout, "%s\n",  symboltable[0x0001] );
	fprintf( stdout, "%s\n",  symboltable[0x0002] );
	fprintf( stdout, "%s\n",  symboltable[0x0003] );
	fprintf( stdout, "%s\n",  symboltable[0x0004] );
	
	for ( int i = 0 ; i < 16 ; i++ ) {
		fprintf( stdout, "%s\n", symboltable[i] );
	}

	fprintf( stdout, "%s\n", symboltable[0x4000] );
	fprintf( stdout, "%s\n", symboltable[0x6000] );

	fprintf( stdout, "User definition symbol\n" );
	for ( int i = 0 ; i < symbolcnt ; i++ ) {
		fprintf( stdout, "[%d] = %s\n", i, symboltable[i] );
	}
}
