// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.
	// @2の値を初期化
	@2
	M=0
// @0の値をセットした後, その値を@1回分足す
// それぞれのループの最初の部分で値が0かどうかで
// 分岐先を決定する
(LOOP)
	// @0の値が0の場合は処理を終了する
	@0
	D=M
	@END
	D; JEQ

	// @1の値が0の場合は処理を終了する
	@1
	D=M
	@END
	D; JEQ

	@0
	D=M

	// @0の値をDレジスタに代入
	@2

	// 途中和とDレジスタの値を足した値で途中和の値を更新する
	M=D+M

	// 繰り返し回数をデクリメントする
	@1
	D=M
	D=D-1
	M=D

	@LOOP
	0; JMP
(END)
	@END
	0; JMP
