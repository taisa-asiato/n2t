#include "define.h"

void callPushLocalFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
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
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushArgumentFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@ARG\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushThisFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THIS\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushThatFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THAT\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushPointerFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@3\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushTempFunction( int index ) {
	// やることはM[@SP] = M[@lcl+index]
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@5\n"  );
	fprintf( outputfp, "A=M+D\n" ); //A=M[@lcl]+index
	fprintf( outputfp, "D=M\n" );

	// M[@SP]へ値を保存する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// @SPの指す値を更新
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPushConstantFunction( int index ) {
	// SPレジスタが指すメモリアドレスには，メモリに記録された定数の数を記録する
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
}

void callPopLocalFunction( int index ) {
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
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
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );
}

void callPopThisFunction( int index ) {
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@THIS\n"  );
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
	fprintf( outputfp, "M=D\n" );;

	//pop後@SPの指すアドレスを1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "D=M-1\n" );
	fprintf( outputfp, "M=D\n" );


}

void callPopThatFunction( int index ) {
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
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
	// tempはthatおよびthisのベースアドレスを
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@3\n"  );
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

void callPopPointerFunction( int index ) {
	// tempはthatおよびthisのベースアドレスを
	// やることはM[index+LCL] = M[@SP-1]
	// @SP部分に格納するアドレスの値を仮に保存しておく
	fprintf( outputfp, "@%d\n", index );
	fprintf( outputfp, "D=A\n" );
	fprintf( outputfp, "@5\n"  );
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


void callPopConstantFunction( index ) { ; }
