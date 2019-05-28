#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 入力ファイルへのファイルポインタ
extern FILE * fp;

// 入力ファイルの名前
extern char fname[256];

// 入力ファイルの中身
extern char str[256];


////////////////
/* parserMain */
////////////////
void parserMain();
bool hasMoreCommands();
int commandType(); 
char * symbol();
char * dest();
