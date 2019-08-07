#include "define.h"

void callAddFunction() {
	// @SPレジスタが指すメモリアドレスに格納された
	// 定数の数を判定する。その定数の個数登録順番と合致している
	// 実装したadd関数では, @SPの指す数（アドレス）とその1個前のアドレス
	// に保存された数の和を求める。
	// 求めた数は1個前のメモリアドレス位置に保存される
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "M=D+M\n");
	// 最後にメモリに登録されている変数の個数を1減らす
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
}
void callSubFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "M=M-D\n");
	// callAddFunctionと同じ
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
}

void callNegFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "M=-M\n" );
//	fprintf( outputfp, "@SP\n" );
//	fprintf( outputfp, "M=M-1\n" );
}

void callEqFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M-D\n");
	// 上の5つの文はただの引き算と同様
	// 書きについては，引き算を行い0でない場合には
	// -1の値を指定されたメモリアドレスへ書き込む
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" ); // 比較結果の値を入れるメモリアドレスを計算し直す
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "@ZEROEQ%d\n", eqnum );
	fprintf( outputfp, "D;JEQ\n" );

	// 値が等しくない場合, Dレジスタに0をセットして代入関数部分へジャンプ
	fprintf( outputfp, "D=0\n" );
	fprintf( outputfp, "@INSERTEQVAL%d\n", eqnum );
	fprintf( outputfp, "0;JMP\n" );

	// 値が等しい場合, Dレジスタに-1をセットして代入関数部分へジャンプする
	fprintf( outputfp, "(ZEROEQ%d)\n", eqnum); 
	fprintf( outputfp, "D=-1\n" );
	fprintf( outputfp, "@INSERTEQVAL%d\n", eqnum );
	fprintf( outputfp, "0;JMP\n" );

	// 比較結果をメモリアドレスに格納する
	fprintf( outputfp, "(INSERTEQVAL%d)\n", eqnum );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );
	// 次回登録できるメモリアドレスの位置を再計算する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );


	eqnum++;
}

void callGtFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M-D\n");
	// 引き算終了
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "@GT%d\n", gtnum );
	fprintf( outputfp, "D;JGT\n" );

	fprintf( outputfp, "D=0\n" );
	fprintf( outputfp, "@INSERTGTVAL%d\n", gtnum );
	fprintf( outputfp, "0;JMP\n" );

	fprintf( outputfp, "(GT%d)\n", gtnum );
	fprintf( outputfp, "D=-1\n" );
	fprintf( outputfp, "@INSERTGTVAL%d\n", gtnum );
	fprintf( outputfp, "0;JMP\n" );

	fprintf( outputfp, "(INSERTGTVAL%d)\n", gtnum );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" ); 
	fprintf( outputfp, "M=D\n" );
	// 次回登録できるメモリアドレスの位置を再計算する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );


	gtnum++;
}

void callLtFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "D=M-D\n");
	// 引き算終了
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "M=M-1\n" );
	fprintf( outputfp, "@LT%d\n", ltnum );
	fprintf( outputfp, "D;JLT\n" );

	fprintf( outputfp, "D=0\n" );
	fprintf( outputfp, "@INSERTLTVAL%d\n", ltnum );
	fprintf( outputfp, "0;JMP\n" );

	fprintf( outputfp, "(LT%d)\n", ltnum );
	fprintf( outputfp, "D=-1\n" );
	fprintf( outputfp, "@INSERTLTVAL%d\n", ltnum );
	fprintf( outputfp, "0;JMP\n" );

	fprintf( outputfp, "(INSERTLTVAL%d)\n", ltnum );
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M\n" );
	fprintf( outputfp, "M=D\n" );

	// 次回登録できるメモリアドレスの位置を再計算する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M+1\n" );
	ltnum++;
}

void callAndFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "M=D&M\n" );
	// 次回値を格納できるメモリアドレス位置が変わるため
	// メモリアドレスを再計算する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );
}

void callOrFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "D=M\n" );
	fprintf( outputfp, "A=A-1\n" );
	fprintf( outputfp, "M=D|M\n" );
	// 次回値を格納できるメモリアドレス位置が変わるため
	// メモリアドレスを再計算する
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "M=M-1\n" );

}

void callNotFunction() {
	fprintf( outputfp, "@SP\n" );
	fprintf( outputfp, "A=M-1\n" );
	fprintf( outputfp, "M=!M\n" );
}

