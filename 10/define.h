#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>

#define NOTDIR		0
#define ISDIR		1

#define KEYWORD 	1
#define SYMBOL		2
#define IDENTIFIER	3
#define INT_CONST	4
#define STRING_CONST	5

// コマンドライン引数として入力されるファイル名を保持する
extern char inputfilename[256];
// 入力ストリームのファイル名
extern char streamfilename[256];
// 入力ストリームへのファイルポインタ
extern FILE * inputfp;
// 出力ファイルネーム
extern char outputfilename[256];
// 出力ファイルストリームへのファイルポインタ
extern FILE * outputfp;
// 入力ストリームライン
extern char streamline[256];
// 現在のトークン
extern char token[256];


/* JackAnalyzer.c */
int gen_inputfilename( struct dirent * dp, char * dirname );
void jack_analyze_main( char *fname );
void gen_outputfilename( char * filename );

/* JackTokenizer.c */
void jack_tokenizer_main( FILE * ifp, FILE * ofp );
bool has_more_tokens( char * istoken );
void advance();
