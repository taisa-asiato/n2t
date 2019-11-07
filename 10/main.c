#include "define.h"

char inputfilename[256];
char streamfilename[256];
FILE * inputfp;
char outputfilename[256];
FILE * outputfp;
char streamline[256];
char token[256];
char keyword_str[21][20] = {
	"class", "method", "function", "constructor",
	"int", "boolean", "char", "void", 
	"var", "static", "field", "let",
	"do", "if", "else", "while", 
	"return", "true", "false", "null", "this"
};
char t_type[256];

int main( int argc, char ** argv ) {
	/* エラー時の処理 */
	if ( argc != 2 ) {
		fprintf( stderr, "[Usage]:JackAnalyze filename\n" );
		return 0;
	} else {
		jack_analyze_main( argv[1] );
	}
}

