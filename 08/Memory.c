#include "define.h"

void callPushLocalFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	// printoutCommentMessage( "push local" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@LCL\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	// fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushArgumentFunction( int index ) {
	// やることはM[@SP] = M[@arg+index]
	
	// printoutCommentMessage( "push argument" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@ARG\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@arg]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushThisFunction( int index ) {
	// 他lclと同様
	
	// printoutCommentMessage( "push this" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THIS\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@this]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushThatFunction( int index ) {

	// printoutCommentMessage( "push that" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THAT\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@that]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushPointerFunction( int index ) {

	// printoutCommentMessage( "push pointer" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@3\n"  );
	fprintf( outputfp, "A=A+D\n" ); //A=M[@pointer]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushTempFunction( int index ) {

	//printoutCommentMessage( "push temp" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@5\n"  );
	fprintf( outputfp, "A=A+D\n" ); //A=M[@temp]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushConstantFunction( int index ) {
	// SPレジスタが指すメモリアドレスには，メモリに記録された定数の数を記録する
	//
	// printoutCommentMessage( "push constant" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );

	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
	
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushStaticFunction( int index ) {
	// シンボルで刺されたアドレスの値をDレジスタに保存する
	//
	// printoutCommentMessage( "push static" );
	fprintf( outputfp, "@%s.%d\n", fnameex2, index );
	fprintf( outputfp, "D=M\n" );
	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPopLocalFunction( int index ) {
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
	//
	// printoutCommentMessage( "pop local" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@LCL\n"  );
	fprintf( outputfp, "D=M+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

void callPopArgumentFunction( int index ) {
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
	//
	// printoutCommentMessage( "pop argument" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@ARG\n"  );
	fprintf( outputfp, "D=M+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
}

void callPopThisFunction( int index ) {

	// printoutCommentMessage( "pop this" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THIS\n"  );
	fprintf( outputfp, "D=M+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" ); // M[@SP]=格納先アドレス

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );;

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );


}

void callPopThatFunction( int index ) {
	// @SP部分に格納するアドレスの値を仮に保存しておく
	//
	/// printoutCommentMessage( "pop that" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THAT\n"  );
	fprintf( outputfp, "D=M+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

void callPopTempFunction( int index ) {
	// 番目のRAMアドレスを指す。
	
	//printoutCommentMessage( "pop temp" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@5\n"  );
	fprintf( outputfp, "D=A+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

void callPopPointerFunction( int index ) {

	// printoutCommentMessage( "pop pointer" );
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@3\n"  );
	fprintf( outputfp, "D=A+D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

void callPopStaticFunction( int index ) {

	// printoutCommentMessage( "pop pointer" );
	fprintf( outputfp, "@%s.%d\n", fnameex2, index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 上記で計算したアドレスに格納するデータをDレジスタに保持させる
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M\n" ); // D=M[@SP-1]

	// @SPが指すアドレスにはデータ格納先のアドレスが, @SP-1には格納するデータが
	// 保存されているので，代入を行う
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

