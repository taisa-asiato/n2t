#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>

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
#define NONE 		21
#define ARG		22

typedef struct __subroutine_list {
	struct __subroutine_list * next;
	struct __subroutine_list * prev;
	char subroutine_name[256];
} subroutine_name_t;

typedef struct __list {
	struct __list * next;
	struct __list * prev;
	char symbol_name[256];
	subroutine_name_t * subrot_head;
	subroutine_name_t * subrot_end;
} list_t;

typedef struct _scope {
	char name[256];
	char type[256];
	char proper[256];
	int number;
	int defined;
	int used;
	struct _scope * next;
	struct _scope * prev;
} scope_t;

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
//
extern char keyword_str[21][20];
// トークンの種類を示す文字列
extern char t_type[256];
// シンボルを管理するリストの先頭を表す
extern list_t * head;
// シンボルを管理するリストの最後を表す
extern list_t * end;
// デバッグ情報出力, 1:出力, 0:出力しない
extern int debug;
// 出力先情報, 0:ファイルへ出力, 1:標準出力
extern int isstdout;
// 関数名の管理するリストの先頭を指す
extern scope_t * cls;
extern scope_t * clsp;
extern scope_t * sub;
extern scope_t * subp;
// 現在のトークンが指す識別子のスコープ情報などを保持する[
extern int cnt_static;
extern int cnt_field;
extern int cnt_arg;
extern int cnt_var;
extern int kind;
extern int symbol_define;
extern int symbol_used;
extern char my_typeof[256];
extern char propof[256];
extern int current_type;
extern char sname[256];
extern char thisclassname[256];

/* JackAnalyzer.c */
int gen_inputfilename( struct dirent * dp, char * dirname );
void jack_analyze_main( char *fname );
void gen_outputfilename( char * filename );

/* JackTokenizer.c */
void jack_tokenizer_main( FILE * ifp, FILE * ofp );
bool has_more_tokens( FILE * fp );
int advance( FILE * ifp );
int kyeword( char current[256] );
bool is_keyword( char c_token[256] );
void symbol( char symbol_string[256] );
void identifier( char identifier_string[256] );
int int_val( char intval_string[256] );
void string_val( char val_string[256] );
int token_type( char current[256] );
bool is_symbol( char c_token[256] );
int keyword( char c_token[256] );

void get_stringconst( FILE * fp );
bool is_identifier( char val_string[256] );
bool is_string_constant( char c_token[256] );
bool is_integer_constant( char c_token[256] );

/* CompliationEngine.c */
void compile_main( FILE * ifp, FILE * ofp );
int compile_Class( FILE * ifp, FILE * ofp, int depth );
int compile_Class_Var_Dec( FILE * ifp, FILE * ofp, int depth );
int compile_Subroutine_Dec( FILE * ifp, FILE * ofp, list_t * class_pos, int depth );
int compile_Statements( FILE * ifp, FILE * ofp, int depth );
int compile_Var_Dec( FILE * ifp, FILE * ofp, int depth );
int compile_Let_Statement( FILE * ifp, FILE * ofp, int depth );
void compile_If_Statement( FILE * ifp, FILE * ofp, int depth );
void compile_While_Statement( FILE * ifp, FILE * ofp, int depth );
void compile_Do_Statement( FILE * ifp, FILE * ofp, int depth );
void compile_Subroutine_Call( FILE * ifp, FILE * ofp, list_t * class_pos, int depth );
int compile_Return_Statement( FILE * ifp, FILE * ofp, int depth );
void compile_Expression( FILE * ifp, FILE * ofp, int depth );
void compile_Term( FILE * ifp, FILE * ofp, int depth );
char compile_Symbol( FILE * ifp, FILE * ofp, char sym, int depth );
void ungets( FILE * ifp, int length );
int compile_ParameterList( FILE * ifp, FILE * ofp, int depth );
int compile_Expression_List( FILE * ifp, FILE * ofp, int depth );
void printTab( FILE * ofp, int depth );
void printTokenAndTag( FILE * ofp, char * type, char * thistoken, int depth );
void printTokenAndTagStart( FILE * ofp, char * thistoken, int depth );
void printTokenAndTagEnd( FILE * ofp, char * thistoken, int depth );
void printTokenStatus( FILE * ofp, char * thistoken, int depth );
void printSubrotStatus( FILE * ofp, list_t * class_p, char * thistoken, int depth );
void printClassStatus( FILE * ofp, list_t * class_p, char * thistoken, int depth );

/* list.c */
void list_Add( char * sym_name );
void list_Init();
void list_Print();
void list_Print_Back();
void list_Delete();
list_t * list_Find_Node( char * f_str );
void list_Make_Standard_Class();
void list_Init_Subrot( list_t * class_pos );
void list_Add_Subrot( list_t * class_pos, char * subrot_name );
void list_Print_Subrot( list_t * class_name );
void list_Print_Back_Subrot( list_t * class_name );
void list_Delete_Subrot( list_t * class_name );
void list_Init_Math_Class( list_t * math_pos );
void list_Init_String_Class( list_t * string_pos );
void list_Init_Array_Class( list_t * array_pos );
void list_Init_Output_Class( list_t * output_pos );
void list_Init_Screen_Class( list_t * screen_pos );
void list_Init_Keyboart_Class( list_t * keyboard_pos );
void list_Init_Memory_Class( list_t * memory_pos );
void list_Init_Sys_Class( list_t * sys_pos );
subroutine_name_t *  list_Find_Node_Subrot( list_t * class_name, char * subrot_name );
list_t * list_Find_Node_Subrot_BelongClass( char * subrot_name );
subroutine_name_t * list_Find_Node_Subrot_NoClass( char * subrot_name );

/* SymbolTable.c */
void init_SymbolTable();
void init_SubroutineTable();
void constructor();
void my_define( int iscls, char * symbol_name, char * type, char * proper, int number );
int var_Count( char * symbol_name );
int kind_Of( char * name );
int type_Of( char * name );
int index_Of( char * name );
void del_SymbolTable();
void del_SubroutineTable();
void print_All_Symbol();
void print_Class_Subrot( char * symbol_name, int iscls );
int Is_Used( char * thistoken );
int Is_Defined( char * thistoken );
scope_t * list_Find_Scope_Cls( char * symbol_name );
scope_t * list_Find_Scope_Sub( char * symbol_name );
scope_t * list_Find_Scope( char * symbol_name );
void delete_SubroutineTable();

/* VMWriter.c */
void writePush( FILE * ofp, int SEGNUMBER, int index );
