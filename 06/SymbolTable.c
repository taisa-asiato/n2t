#include "define.h"

void SymbolTableInit() {
	char cpystr[4];
	int i = 0;
	
	for ( i = 0 ; i < 0xFFFF ; i++ ) {
		symboltable[i].symbolname[0] = '\0';
		symboltable[i].symboladdress = -1;
	}

	for ( i = 0 ; i < 16 ; i++ ) {
		sprintf( cpystr, "R%d", i );
		strcpy( symboltable[i].symbolname, cpystr );
		symboltable[i].symboladdress = i;
	}


	strcpy( symboltable[i].symbolname, "SCREEN" );
	symboltable[i].symboladdress = 0x4000;

	i++;
	strcpy( symboltable[i].symbolname, "KBD" );
	symboltable[i].symboladdress = 0x6000;

	/* Special Symbolのテーブルを別途作成 */
	i++;
	strcpy( symboltable[i].symbolname, "SP" );
	symboltable[i].symboladdress = 0x0000;

	i++;
	strcpy( symboltable[i].symbolname, "LCL" );
	symboltable[i].symboladdress = 0x0001;

	i++;
	strcpy( symboltable[i].symbolname, "ARG" );
	symboltable[i].symboladdress = 0x0002;

	i++;
	strcpy( symboltable[i].symbolname, "THIS" );
	symboltable[i].symboladdress = 0x0003;

	i++;
	strcpy( symboltable[i].symbolname, "THAT" );
	symboltable[i].symboladdress = 0x0004;

	reg = i + 1;
}

void addEntry( char * symbol, int address ) {
	strcpy( symboltable[reg].symbolname, symbol );
	symboltable[reg].symboladdress = address;
}

bool contains( char * symbol ) {
	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		if ( strcmp( symboltable[i].symbolname, symbol ) == 0 ) {
			return true;
		}
	}
/*
	for ( int i = 0 ; i < 5 ; i++ ) {
		if ( strcmp( specialsymbol[i], symbol ) == 0 ) {
			return true;
		}
	}
*/
	return false;
}

int getAddress( char * symbol ) {
	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		if ( strcmp( symboltable[i].symbolname, symbol ) == 0 ) {
			return symboltable[i].symboladdress;
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
	for ( int i = 0 ; i < 0xFFFF ; i++ ) {
		if ( symboltable[i].symboladdress == -1 || symboltable[i].symbolname[0] == '\0' ) {
			break;
		}
		fprintf( stdout, "[%5d] %-50s = %d\n", i, 
				symboltable[i].symbolname, symboltable[i].symboladdress );
	}
}
