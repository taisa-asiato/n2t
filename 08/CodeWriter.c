#include "define.h"

void setFileName( int count ) {
	if ( count == 0 ) {
		outputfp = fopen( outputfilename, "w" );
	} else {
		outputfp = fopen( outputfilename, "a" );
	}
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
	fprintf( outputfp, "(Sys.init)\n" );
	fprintf( outputfp, "0;JMP\n" );
}

////////////////////////////////////////////////////
/* プログラムフローVMコードをアセンブラに変換する */
////////////////////////////////////////////////////
void writeLabel( char * label ) {
	// printoutCommentMessage( "label" );
	fprintf( outputfp, "(%s)\n", label );
}


void writeGoto( char * label ) {
	// printoutCommentMessage( "goto label" );
	fprintf( outputfp, "@%s\n", label );
	fprintf( outputfp, "0;JMP\n" );
}

void writeIf( char * label ) {
	// スタックポインタの指すアドレスに格納された値を
	// ポップし, その値で関数へJMPするか決定する
	// printoutCommentMessage( "if-goto label" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" ); // A=M[@SP]-1
	fprintf( outputfp, "D=M\n" ); // D=M[A]
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "@%s\n", label );
	fprintf( outputfp, "D;JNE\n" );
}

//////////////////////////////////////////////
/* 関数コールVMコードをアセンブラに変換する */
//////////////////////////////////////////////
void writeCall( char * functionName, int numArgs ) {
	// ラベルをスタックポインタの指すアドレスへ格納する
	callPushLabelValue( functionName );
	// 呼び出し側の各レジスタの1指すメモリアドレスを順次格納していく
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
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@LCL\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// ラベルを宣言する
	fprintf( outputfp, "(%s)\n", functionName );
}

void writeReturn() {
	// LCLのリターンアドレスを取得する
	fprintf( outputfp, "@LCL\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@FRAME\n" ); // 一時変数を仕様できるメモリアドレスを設定する
	// fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" ); //M[FRAME]=D

	// 関数の戻り値を別の場所へ移動する
	callPopArgumentFunction( 0 ); // *ARG=pop()
	fprintf( outputfp, "@ARG\n" ); // SP=ARG+1
	fprintf( outputfp, "D=M+1\n" ); // 
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=D\n" );

	callRestoreMemoryValue( "THAT", 1 ); // THAT=*(FRAME-1)
	callRestoreMemoryValue( "THIS", 2 ); // THIS=*(FRAME-2)
	callRestoreMemoryValue( "ARG",  3 ); // ARG =*(FRAME-3)
	callRestoreMemoryValue( "LCL",  4 ); // LCL =*(FRAME-4)

	fprintf( outputfp, "@FRAME\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@5\n" );
	fprintf( outputfp, "A=D-A\n" ); // goto RET
	fprintf( outputfp, "0;JMP\n" );
}

void writeFunction( char * functionName, int numArgs ) {
	
	fprintf( outputfp, "(%s)\n", functionName );
	for ( int i = 0 ; i < numArgs ; i++ ) {
		callPushConstantFunction( 0 ); // numArgs回0pushを行う
	}
}

void callPushLabelValue( char * labelname ) {
	fprintf( outputfp, "@%s\n", labelname );
	fprintf( outputfp, "D=A\n" ); // D=A
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );   // A=M[0]
	fprintf( outputfp, "M=D\n" );  // M[0]=D
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callRestoreMemoryValue( char * label, int num ) {
	fprintf( outputfp, "@FRAME\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@FRAME]
	fprintf( outputfp, "@%d\n", num );
	// A=D-A ( M[@FRAME] - A ) FRAMEに保存されたメモリアドレス位置から
	// num分ひいたメモリアドレス位置の値をlabel位置のレジスタに格納する
	fprintf( outputfp, "A=D-A\n" ); 
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@%s\n", label );
	fprintf( outputfp, "M=D\n" );
}

void printoutCommentMessage( char * contents ) {
	fprintf( outputfp, "//%s\n",  contents);
}
