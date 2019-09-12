#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

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

/* JackAnalyzer.c */
int gen_inputfilename( struct dirent * dp );
void jack_analyze_main( char ** fname );
void gen_outputfilename( char * dirname, char * filename );

/* JackTokenizer.c */
void jack_tokenizer_main();
