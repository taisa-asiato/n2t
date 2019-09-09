#include "define.h"

char inputfilename[256];
char streamfilename[256];


int main( int argc, char ** argv ) {
	DIR * dirp;
	struct dirent * dp;

	/* エラー時の処理 */
	if ( argc != 2 ) {
		fprintf( stderr, "[Usage]:JackAnalyze filename\n" );
		return 0;
	}

	if ( ( dirp = opendir( argv[1] ) ) != NULL ) {
		// 入力ファイルがディレクトリの場合
		while ( ( dp = readdir( dirp ) ) != NULL ) {
			// 入力ファイル名を作成
			if ( gen_filename( dp ) ) {
				fprintf( stdout, "%s\n", streamfilename );
			}
		}

		// 入力ストリーム
	} else {
		// 入力ファイルがディレクトリでない場合
		strcpy( streamfilename, argv[1] );
		fprintf( stdout, "%s\n", streamfilename );
	}

}

int gen_filename( struct dirent * dp ) {
	int isjack = 0;
	if ( strstr( dp->d_name, ".jack" ) ) {
		strcpy( streamfilename, dp->d_name );
		isjack = 1;
	}
	return isjack;
}
