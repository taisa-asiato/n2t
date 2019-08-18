#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

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
extern char inputfilename[256];
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
// スタック領域
extern int stack[0xff];
// ヒープ領域
extern int heap[0xff];
// 出力先ファイルへのファイルポインタ
extern FILE * outputfp;
// 出力先ファイルネーム
extern char outputfilename[1000];
// 各算術コマンドの数
extern unsigned int eqnum;
extern unsigned int ltnum;
extern unsigned int gtnum;
extern char fnameex2[256];

//////////////
/* Parser.c */
//////////////
bool hasMoreCommands();
void advance();
int commandType();
char * arg1();
int arg2();
void VMTransMain( int count );
void InitAll();
void PrintAscii( char ** str );
void InitCommand();
void makeOutputFilename();
void makeFileName( char * dirname, char * filename );
 
//////////////////
/* CodeWriter.c */
//////////////////
void setFileName( int count );
void writeArithmetic( char * command );
void writePushPop( int command, char * segment, int index );
void close();
// >>> 8章の追加関数
void writeInit();
void writeLabel( char * label );
void writeGoto( char * label );
void writeIf( char * label );
void writeCall( char * functionName, int numArgs );
void writeReturn();
void writeFunction( char * functionName, int numArgs );
// シンボルの値を簡単にpushできるようサブルーチン化
void callPushLabelValue( char * labelname );
void callRestoreMemoryValue( char * label, int num );
void printoutCommentMessage( char * contents  );

//////////////////
/* Arithmetic.c */
//////////////////
// 算術コマンド用関数
void callAddFunction();
void callSubFunction();
void callNegFunction();
void callEqFunction();
void callGtFunction();
void callLtFunction();
void callAndFunction();
void callOrFunction();
void callNotFunction();

//////////////
/* Memory.c */
//////////////
// セグメント格納用関数
void callPushConstantFunction( int index );
void callPushLocalFunction( int index );
void callPushArgumentFunction( int index );
void callPushThisFunction( int index );
void callPushThatFunction( int index );
void callPushPointerFunction();
void callPushTempFunction();
void callPushStaticFunction( int index );

void callPopLocalFunction( int index );
void callPopArgumentFunction( int index );
void callPopThisFunction( int index );
void callPopThatFunction( int index );
// void callPopConstantFunction( int index );
void callPopTempFunction( int index );
void callPopPointerFunction( int index );
void callPopStaticFunction( int index );
