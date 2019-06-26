#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"

// 各コマンド
#define A_COMMAND 1
#define C_COMMAND 2
#define L_COMMAND 3
#define E_CMDERR -1
#define E_COMMENT -2
#define E_BLANK -3


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

////////////
/* Code.c */
////////////
char * CodeDest();
char * CodeComp();
char * CodeJump();
void DestDataInit();
void JumpDataInit();
void CompDataInit();

///////////////////
/* SymnolTable.c */
///////////////////
void addEntry( char * symbol, char * address );
bool contains( char * symbol );
int getAddress( char * symbol );

/////////////////
/* Assembler.c */
/////////////////
char *  IntegerToBinaryString( char integer[256] );
