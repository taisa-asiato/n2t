#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

// コマンドライン引数として入力されるファイル名を保持する
extern char inputfilename[256];
// 入力ストリームのファイル名
extern char streamfilename[256];

/* main.c */
int gen_filename( struct dirent * dp );

