#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>

#define NOTDIR		0
#define ISDIR		1

// 
#define KEYWORD 	1
#define SYMBOL		2
#define IDENTIFIER	3
#define INT_CONST	4
#define STRING_CONST	5

//
#define CLASS		0
#define METHOD		1
#define FUNCTION	2
#define CONSTRUCTOR	3
#define INT		4
#define BOOLEAN		5
#define CHAR		6
#define VOID		7
#define VAR		8
#define	STATIC		9
#define	FIELD		10
#define	LET		11
#define	DO		12
#define	IF		13
#define	ELSE		14
#define	WHILE		15
#define	RETURN		16
#define	TRUE		17
#define	FALSE		18
#define	exNULL		19
#define THIS		20

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
int kyeword( char current[256] );
bool is_keyword( char c_token[256] );
void symbol( char symbol_string[256] );
void identifier( char identifier_string[256] );
int int_val( char intval_string[256] );
void string_val( char val_string[256] );
int token_type( char current[256] );
bool is_symbol( char c_token[256] );
int keyword( char c_token[256] );

bool is_identifier( char val_string[256] );
bool is_string_constant( char c_token[256] );
bool is_integer_constant( char c_token[256] );
