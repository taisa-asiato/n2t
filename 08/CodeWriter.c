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
	// fprintf( stdout, "Initiallize\n" );
	// call Sys.initを行う
	writeGoto( "Sys.init" );
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
	char ret[256];

	// ラベルが指すアドレスをスタックポインタの指すアドレスへ格納する
	sprintf( ret, "%s$RET.%d", functionName, retval );
	callPushLabelValue( ret );

	// 呼び出し側の各レジスタの1指すメモリアドレスを順次格納していく
	callPushLabelLATT( "LCL" );
	callPushLabelLATT( "ARG" );
 	callPushLabelLATT( "THIS" );
	callPushLabelLATT( "THAT" );

	// ARGの指すメモリアドレスの値を変更する
	// ARG = SP-n-5
	fprintf( outputfp, "@SP\n" ); // A=0
	// fprintf( outputfp, "A=M\n" ); // <<< 変更点　要確認
	fprintf( outputfp, "D=M\n" ); // D=M[A]
	for ( int i = 0 ; i < numArgs+5 ; i++ ) {
		fprintf( outputfp, "D=D-1\n" );
	}
	// fprintf( outputfp, "@%d\n", numArgs+5 ); // A=numArgs
	// fprintf( outputfp, "D=D-A\n" ); // D=D-A
	fprintf( outputfp, "@ARG\n" ); // 
	// fprintf( outputfp, "A=M\n" ); // A=M[@ARG]
	fprintf( outputfp, "M=D\n" ); // M[@ARG]=D

	// LCLの指すメモリアドレスの値を変更する
	// LCL = SP
	fprintf( outputfp, "@SP\n" );
	// fprintf( outputfp, "A=M\n" ); // <<< 要確認
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@LCL\n" );
	// fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
	// goto-f
	writeGoto( functionName );

	// ラベルを宣言する
	writeLabel( ret );
	retval++;
}

void writeReturn() {

	char retaddr[256];
	sprintf( retaddr, "RET_%d", framenum );
	// LCLのリターンアドレスを取得する
	// FRAME = LCL
	fprintf( outputfp, "@LCL\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@FRAME$%d\n", framenum ); // 一時変数を仕様できるメモリアドレスを設定する
	fprintf( outputfp, "M=D\n" ); //M[FRAME]=D

	// RET = *(FRAME-5)
	fprintf( outputfp, "@FRAME$%d\n", framenum );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@5\n" );
	fprintf( outputfp, "A=D-A\n" ); // goto RET
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@%s\n", retaddr );
	fprintf( outputfp, "M=D\n" );

	// 関数の戻り値を別の場所へ移動する
	// *ARG = pop()
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "@ARG\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );

	// SP=ARG+1
	fprintf( outputfp, "@ARG\n" ); //
	fprintf( outputfp, "D=M+1\n" ); // 
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=D\n" );

	callRestoreMemoryValue( "THAT", 1 ); // THAT=*(FRAME-1)
	callRestoreMemoryValue( "THIS", 2 ); // THIS=*(FRAME-2)
	callRestoreMemoryValue( "ARG",  3 ); // ARG =*(FRAME-3)
	callRestoreMemoryValue( "LCL",  4 ); // LCL =*(FRAME-4)
	
	// goto RET
	fprintf( outputfp, "@%s\n", retaddr );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "0;JMP\n" );
	framenum++;
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

void callPushLabelLATT( char * labelname ) {
	fprintf( outputfp, "@%s\n", labelname );
//	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@labelname]
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );   // A=M[0]
	fprintf( outputfp, "M=D\n" );  // M[0]=D
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callRestoreMemoryValue( char * label, int num ) {
	fprintf( outputfp, "@FRAME$%d\n", framenum );
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
