#include "define.h"

void setFileName( char * outputfilename ) {
	outputfp = fopen( outputfilename, "w" );
}

void writeArithmetic( char * command ) {

	char convstr[256];

	// 各算術用VMコマンドを所望の動作を行うhackアセンブラに
	// 変換suru
	if ( strcmp( command, "add" ) == 0 ) {
		callAddFunction();
	} else if ( strcmp( command, "sub" ) == 0 ) {
		fprintf( outputfp, "M=D-M\n" );
	} else if ( strcmp( command, "neg" ) == 0 ) {
		fprintf( outputfp, "-M\n" );
	} else if ( strcmp( command, "eq" ) == 0 ) { 
		fprintf( outputfp, "JLE\n" );
	} else if ( strcmp( command, "gt" ) == 0 ) {
		fprintf( outputfp, "JGT\n" );
	} else if ( strcmp( command, "lt" ) == 0 ) {
		fprintf( outputfp, "JLT\n" );
	} else if ( strcmp( command, "and" ) == 0 ) {
		fprintf( outputfp, "D&M\n" );
	} else if ( strcmp( command, "or" ) == 0 ) {
		fprintf( outputfp, "D|M\n" );
	} else if ( strcmp( command, "not" ) == 0 ) {
		fprintf( outputfp, "!M\n" );
	}
}

void writePushPop( int command, char * segment, int index ) {
	if ( command == C_POP ) {
		;
	} else if ( command == C_PUSH ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			;	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			;
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			;
		} else if ( strcmp( segment, "that" ) == 0 ) {
			;
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			;
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			;
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			callConstantFunction( index );
		} else if ( strcmp( segment, "static" ) == 0 ) {
			fprintf( outputfp, "@xxx.%d\n", index );
			fprintf( outputfp, "D=M\n" );
		}
	} else {
		fprintf( stdout, "ERROR: command is invalid value\n" );
	}
}

void close() {
	fclose( outputfp );
}

void callAddFunction() {
	// @SPレジスタが指すメモリアドレスに格納された
	// 定数の数を判定する。その定数の個数登録順番と合致している
	// 実装したadd関数では, @SPの指す数（アドレス）とその1個前のアドレス
	// に保存された数の和を求める。
	// 求めた数は1個前のメモリアドレス位置に保存される
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "M=D+M\n");
}

void callConstantFunction( int index ) {
	// SPレジスタが指すメモリアドレスには，メモリに記録された定数の数を記録する
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
}
