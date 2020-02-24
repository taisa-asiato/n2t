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
list_t * head;
list_t * end;
// デバッグ情報を出力する場合, 1をセットしコンパイルする
int debug = 0;
// 出力先を変更する場合, 1をセットしコンパイルする
int isstdout = 1;

// 識別子の型や属性及びインデックスを登録するための変数
scope_t * cls;
scope_t * clsp;

scope_t * sub;
scope_t * subp;
int cnt_static = 0;
int cnt_field = 0;
int cnt_arg = 0;
int cnt_var = 0;
int kind = 0;
int symbol_define = 0;
int symbol_used = 0;
char my_typeof[256];
char propof[256];
int current_type;
char sname[256];
char thisclassname[256];
char opstack[512];
int opnum = 0;

int main( int argc, char ** argv ) {
	/* エラー時の処理 */
	if ( argc != 2 ) {
		fprintf( stderr, "[Usage]:JackAnalyze filename\n" );
		return 0;
	} else {
		list_Init();
		list_Make_Standard_Class();
		jack_analyze_main( argv[1] );
	}

	list_Delete();
}

