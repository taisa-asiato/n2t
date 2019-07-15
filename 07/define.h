#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* マクロ */
#define C_ARITHMETIC	1
#define C_PUSH 		2
#define C_POP		3
#define C_LABEL		4
#define C_GOTO		5
#define C_IF		6
#define C_FUNCTION	7
#define C_RETURN	8
#define C_CALL		9


/* グローバス変数 */
// 本当は使用したくないが, 書籍の使用上このような実装にせざるをえない
// 入力ファイルへのファイルポインタ
extern FILE * fp;
// 入力ファイルネーム
extern char filename[256];
// 入力ファイルの文字列（１行）
extern char line[256];
// 現在のコマンドを保持する
extern char current_cmd[256];
// コマンドのみを格納する
extern char cmd[256];
// 第一引数
extern char argstr1[256];
// 第二引数
extern char argstr2[256];


// 出力先ファイルへのファイルポインタ
extern FILE * outputfp;
// 出力先ファイルネーム
extern FILE * outputfilename[256];

//////////////
/* Parser.c */
//////////////
bool hasMoreCommands();
void advance();
int commandType();
char * arg1();
int arg2();
void ParseMain();
void InitAll();
void PrintAscii( char ** str );
void InitCommand();


//////////////////
/* CodeWriter.c */
//////////////////
void setFileName( char ** outputfilename );
void writeArithmetic( char ** command );
void writePushPop( char ** command, char ** segment, int index );
void close();
