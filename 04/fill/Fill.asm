// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(MAINLOOP)
	@1000
	D=A
	@R0
	M=D

	@KBD
	D=M

	@BLACKLOOP
	D; JGT


(BLACKLOOP)
	// screen部分のアドレスを最大アドレス分まで進めていく
	@KBD
	D=M

	@MAINLOOP
	D; JEQ

	@R0
	D=M

	@BLACK
	0; JMP

	D=D-1
	@R0
	M=D

	@BLACKLOOP
	D; JGT

(BLACK)
	// 指定された先頭ピクセルから16ピクセル
	// 分黒く塗り潰していく
	@SCREEN
	A=A+D

	M=1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1
	M=M+1



// (WHITE)
	// 指定された先頭ピクセルから16ピクセル
	// 分黒く塗り潰していく
//	@SCREEN
//	A=A+D

//(WHITELOOP)
	

//	M=0
	
