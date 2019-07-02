#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

// 各コマンド
#define A_COMMAND 1
#define C_COMMAND 2
#define L_COMMAND 3
#define E_CMDERR -1
#define E_COMMENT -2
#define E_BLANK -3
// 命令がa=0 or a=1か判定する
#define typeA 0
#define typeM 1

// 入力ファイルへのファイルポインタ
extern FILE * fp;

// 入力ファイルの名前
extern char fname[256];

// 入力ファイルの中身
extern char str[256];

extern char retsymbol[256];
// destの返り値 
extern char retdest[10];
// compの返り値
extern char retcomp[10];
// jumpの返り値
extern char retjump[10];
// 現在のコマンドを保持する
extern char current_cmd[256];

// destのbit列を表す
extern char bitdest[8][4];
// compのbit列を表す
extern char bitcomp[18][7];
// jumpのbit列を表す
extern char bitjump[8][4];

// シンボル登録用のテーブル
extern char symboltable[0xFFFF+1][0xFF+1];
extern char specialsymbol[5][10];
// 登録シンボル数のカウンタ
extern int symbolcnt;

////////////////
/* Parser.c   */
////////////////
int parserMain();
bool hasMoreCommands();
int commandType(); 
char * symbol();
char * dest();
char * comp();
char * jump();
void PrintFunctionName( char * funcname );

////////////
/* Code.c */
////////////
char * CodeDest();
char * CodeComp();
char * CodeJump();
void DestDataInit();
void JumpDataInit();
void CompDataInit();
int CodeAorM();

///////////////////
/* SymnolTable.c */
///////////////////
void addEntry( char * symbol, int address );
bool contains( char * symbol );
int getAddress( char * symbol );
void SymbolTableInit();
void PrintTable();

/////////////////
/* Assembler.c */
/////////////////
char *  IntegerToBinaryString( char integer[256] );
int FirstLoop();
int SecondLoop();
bool IsString( char * strsymbol );
void PrintStrASCII();
bool IsContents( int address );
