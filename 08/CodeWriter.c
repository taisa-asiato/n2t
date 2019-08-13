#include "define.h"

void setFileName() {
	outputfp = fopen( outputfilename, "w" );
}

void writeArithmetic( char * command ) {

	char convstr[256];

	// 各算術用VMコマンドを所望の動作を行うhackアセンブラに
	// 変換suru
	if ( strcmp( command, "add" ) == 0 ) {
		callAddFunction();
	} else if ( strcmp( command, "sub" ) == 0 ) {
		// fprintf( outputfp, "M=D-M\n" );
		callSubFunction();
	} else if ( strcmp( command, "neg" ) == 0 ) {
		// fprintf( outputfp, "-M\n" );
		callNegFunction();
	} else if ( strcmp( command, "eq" ) == 0 ) { 
		// fprintf( outputfp, "JLE\n" );
		callEqFunction();
	} else if ( strcmp( command, "gt" ) == 0 ) {
		// fprintf( outputfp, "JGT\n" );
		callGtFunction();
	} else if ( strcmp( command, "lt" ) == 0 ) {
		// fprintf( outputfp, "JLT\n" );
		callLtFunction();
	} else if ( strcmp( command, "and" ) == 0 ) {
		// fprintf( outputfp, "D&M\n" );
		callAndFunction();
	} else if ( strcmp( command, "or" ) == 0 ) {
		// fprintf( outputfp, "D|M\n" );
		callOrFunction();
	} else if ( strcmp( command, "not" ) == 0 ) {
		// fprintf( outputfp, "!M\n" );
		callNotFunction();
	}
}

void writePushPop( int command, char * segment, int index ) {
	if ( command == C_POP ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			callPopLocalFunction( index );	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			callPopArgumentFunction( index );
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			callPopThisFunction( index );
		} else if ( strcmp( segment, "that" ) == 0 ) {
			callPopThatFunction( index );
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			callPopPointerFunction( index );
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			callPopTempFunction( index );
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			; // callPopConstantFunction( index );
		} else if ( strcmp( segment, "static" ) == 0 ) {
			callPopStaticFunction( index );
		}
	} else if ( command == C_PUSH ) {
		if ( strcmp( segment, "local" ) == 0 ) {
			callPushLocalFunction( index );	
		} else if ( strcmp( segment, "argument" ) == 0 ) {
			callPushArgumentFunction( index );
		} else if ( strcmp( segment, "this" ) == 0 ) { 
			callPushThisFunction( index );
		} else if ( strcmp( segment, "that" ) == 0 ) {
			callPushThatFunction( index );
		} else if ( strcmp( segment, "pointer" ) == 0 ) {
			callPushPointerFunction( index );
		} else if ( strcmp( segment, "temp" ) == 0 ) {
			callPushTempFunction( index );
		} else if ( strcmp( segment, "constant" ) == 0 ) {
			callPushConstantFunction( index );
		} else if ( strcmp( segment, "static" ) == 0 ) {
			callPushStaticFunction( index );
		}
	} else {
		fprintf( stdout, "ERROR: command is invalid value\n" );
	}
}

void close() {
	fclose( outputfp );
}


void writeInit() {
	// @SPの値が256を指すようにする
	fprintf( outputfp, "@256\n" );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=D\n" );
	// call Sys.initを行う
	fprintf( outputfp, "@Sys.init\n" );
	fprintf( outputfp, "0;JMP\n" );
}

void writeLabel( char * label ) {
	fprintf( outputfp, "@%s\n", label );
}

void writeGoto( char * lebel ) {
	fprintf( outputfp, "@%s\n", label );
	fprintf( outputfp, "0;JMP\n" );
}

void writeIf( char * lebel ) {
	// スタックポインタの指すアドレスに格納された値を
	// ポップし, その値で関数へJMPするか決定する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" ); // A=M[@SP], Aレジスタにスタックポインタのアドレスを格納
	fprintf( outputfp, "D=M\n" ); // D=M[A]
	fprintf( outputfp, "@%s\n", label );
	fprintf( outputfp, "D;JMP\n" );
}

void writeCall( char * functionName, int numArgs ) {
	// ラベルをスタックポインタの指すアドレスへ格納する
	callPushLableValue( functionName );
	// 呼び出し側の各レジスタの指すメモリアドレスを順次格納していく
	callPushLabelValue( "LCL" );
	callPushLabelValue( "ARG" );
 	callPushLabelValue( "THIS" );
	callPushLabelValue( "THAT" );

	// ARGの指すメモリアドレスの値を変更する
	fprintf( outputfp, "@SP\n" ); // A=0
	fprintf( outputfp, "D=M\n" ); // D=M[A]
	fprintf( outputfp, "@5\n" );  // A=5
	fprintf( outputfp, "D=D-A\n" ); // D=M[@SP]-5
	fprintf( outputfp, "@%d\n", numArgs ); // A=numArgs
	fprintf( outputfp, "D=D-A\n" ); // D=D-A
	fprintf( outputfp, "@ARG\n" ); // 
	fprintf( outputfp, "A=M\n" ); // A=M[@ARG]
	fprintf( outputfp, "M=D\n" ); // M[@ARG]=D

	// LCLの指すメモリアドレスの値を変更する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "" );
}

void callPushLableValue( char * labelname ) {
	fprintf( outputfp, "@%s\n", labelname );
	fprintf( outputfp, "D=A\n" ); // D=A
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );   // A=M[0]
	fprintf( outputfp, "M=D\n" );  // M[0]=D
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}
