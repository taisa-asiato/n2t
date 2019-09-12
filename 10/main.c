#include "define.h"

char inputfilename[256];
char streamfilename[256];
FILE * fp;
char outputfilename[256];
FILE * outputfp;
char streamline[256];


int main( int argc, char ** argv ) {
	/* エラー時の処理 */
	if ( argc != 2 ) {
		fprintf( stderr, "[Usage]:JackAnalyze filename\n" );
		return 0;
	} else {
		jack_analyze_main( argv[1] );
	}
}

