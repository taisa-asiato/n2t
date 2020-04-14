#include "define.h"

void list_Init() {
	head = malloc( sizeof( list_t ) );
	head->next = NULL;
	head->prev = NULL;
}

void list_Init_Subrot( list_t * class_pos ) {
	class_pos->subrot_head = malloc( sizeof( subroutine_name_t ) );
	class_pos->subrot_head->next = NULL;
	class_pos->subrot_head->prev = NULL;
}

void list_Add_Subrot( list_t * class_pos, char * subrot_name ) {
	subroutine_name_t * p;
	subroutine_name_t * tmp = class_pos->subrot_head;
	p = malloc( sizeof( subroutine_name_t ) );

	strcpy( p->subroutine_name, subrot_name );

	while ( tmp->next != NULL ) {
		tmp = tmp->next;
	}

	tmp->next = p;
	p->prev = tmp;
	p->next = NULL;
	class_pos->subrot_end = p;
}

void list_Add( char * sym_name ) {
	list_t * p;
	list_t * tmp = head;
	p = malloc( sizeof( list_t ) );

	strcpy( p->symbol_name, sym_name );

	while ( tmp->next != NULL ) {
		tmp = tmp->next;
	}

	tmp->next = p;
	p->prev = tmp;
	p->next = NULL;
	end = p;
}

void list_Print_Subrot( list_t * class_name ) {
	subroutine_name_t * p = class_name->subrot_head;
	while ( p != NULL ) {
		fprintf( stdout, "\t\t[%p:%40s]\n", p->subroutine_name, p->subroutine_name );
		p = p->next;
	}
}

void list_Print() {
	list_t * p = head;

	while ( p != NULL ) {
		fprintf( stdout, "\t[%10s]\t%p\n", p->symbol_name, p );
		list_Print_Subrot( p );
		p = p->next;
	}
}

void list_Print_Back_Subrot( list_t * class_name ) {
	subroutine_name_t * p = class_name->subrot_end;

	while ( p != NULL ) {
		fprintf( stdout, "\t\t[%50s]\n", p->subroutine_name );
		p = p->prev;
	}
}

void list_Print_Back() {
	list_t * p = end;

	while ( p != NULL ) {
		fprintf( stdout, "%s", p->symbol_name );
		p = p->prev;
	}
}

void list_Delete_Subrot( list_t * class_name ) {
	subroutine_name_t * p = class_name->subrot_end;
	subroutine_name_t * tmp;

	while ( p != NULL ) {
		tmp = p->prev;
		free( p );
		p = tmp;
	}
}

void list_Delete() {
	list_t * p = end;
	list_t * tmp;

	while ( p != NULL ) {
		tmp = p->prev;
		free( p );
		p = tmp;
	}
}

list_t * list_Find_Node( char * f_str ) {
	list_t * p = head;
	list_t * tmp;

	while ( p != NULL ) {
		if ( strcmp( f_str, p->symbol_name) == 0 ) {
			return p;
		}
		p = p->next;
	}
	return 0;
}

subroutine_name_t * list_Find_Node_Subrot( list_t * class_name, char * subrot_name ) {
	subroutine_name_t * p = class_name->subrot_head;
	
	while ( p ) {
		if ( debug ) {
			fprintf( stdout, "[%s]:Start, class name is [%s]: class address is [%p]\n", __func__, class_name->symbol_name, class_name->symbol_name  );
		}
		if ( strcmp( p->subroutine_name, subrot_name ) == 0 ) {
				return p;
		}
		p = p->next;
	}
	return 0;
}

// 関数名から当該関数が宣言されているクラスのリストポインタを返す
list_t * list_Find_Node_Subrot_BelongClass( char * subrot_name ) {
	list_t * p = head;
	subroutine_name_t * tmp;

	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__ );
	}
	while ( p != NULL ) {
		tmp = p->subrot_head;
		while ( tmp != NULL ) {
			if ( strcmp( tmp->subroutine_name, subrot_name ) == 0 ) {
				return p;
			}
			tmp = tmp->next;
		}
		p = p->next;
	}
	return NULL;
}

// 関数名から探索する関数がリストに登録されているか確認する
subroutine_name_t * list_Find_Node_Subrot_NoClass( char * subrot_name ) {
	list_t * p = head;
	subroutine_name_t * tmp;

	if ( debug ) {
		fprintf( stdout, "[%s]:Start\n", __func__ );
	}

	while ( p != NULL ) {
		tmp = p->subrot_head;
		if ( debug ) {
			fprintf( stdout, "%p %p:%s\n", tmp, p, p->symbol_name );
		}
		while ( tmp ) {
			// fprintf( stdout, "%s\n", tmp->subroutine_name );
			if ( strcmp( tmp->subroutine_name, subrot_name ) == 0 ) {
				return tmp;
			}
			tmp = tmp->next;
		}
		p = p->next;
	}
	return NULL;
}

void list_Make_Standard_Class() {
	list_Add( "Math" );
	list_Add( "String" );
	list_Add( "Array" );
	list_Add( "Output" );
	list_Add( "Screen" );
	list_Add( "Keyboard" );
	list_Add( "Memory" );
	list_Add( "Sys" );

	list_t * p = head;
	while ( p != NULL ) {
		if ( debug ) {
			fprintf( stdout, "%s\n", p->symbol_name );
		}
		if ( strcmp( p->symbol_name, "Math" ) == 0 ) {
			list_Init_Math_Class( p );
		} else if ( strcmp( p->symbol_name, "String" ) == 0 ) {
			list_Init_String_Class( p );
		} else if ( strcmp( p->symbol_name, "Array" ) == 0 ) {
			list_Init_Array_Class( p );
		} else if ( strcmp( p->symbol_name, "Output" ) == 0 ) {
			list_Init_Output_Class( p );
		} else if ( strcmp( p->symbol_name, "Screen" ) == 0 ) {
			list_Init_Screen_Class( p );
		} else if ( strcmp( p->symbol_name, "Keyboard" ) == 0 ) {
			list_Init_Keyboart_Class( p );
		} else if ( strcmp( p->symbol_name, "Memory" ) == 0 ) {
			list_Init_Memory_Class( p );
		} else if ( strcmp( p->symbol_name, "Sys" ) == 0 ) {
			list_Init_Sys_Class( p );
		}
		p = p->next;
	}
}

void list_Init_Math_Class( list_t * math_pos ) {
	list_Init_Subrot( math_pos );
	list_Add_Subrot( math_pos, "init" );
	list_Add_Subrot( math_pos,  "abs" );
	list_Add_Subrot( math_pos, "multply" );
	list_Add_Subrot( math_pos, "divide" );
	list_Add_Subrot( math_pos, "min" );
	list_Add_Subrot( math_pos, "max" );
	list_Add_Subrot( math_pos, "sqrt" );
}

void list_Init_String_Class( list_t * string_pos ) {
	list_Init_Subrot( string_pos );
	list_Add_Subrot( string_pos, "new" );
	list_Add_Subrot( string_pos, "dsipose" );
	list_Add_Subrot( string_pos, "length" );
	list_Add_Subrot( string_pos, "charAt" );
	list_Add_Subrot( string_pos, "setCharAt" );
	list_Add_Subrot( string_pos, "appendChar" );
	list_Add_Subrot( string_pos, "eraseLastChar" );
	list_Add_Subrot( string_pos, "intValue" );
	list_Add_Subrot( string_pos, "setInt" );
	list_Add_Subrot( string_pos, "backSpace" );
	list_Add_Subrot( string_pos, "doubleQuote" );
	list_Add_Subrot( string_pos, "newLine" );
}

void list_Init_Array_Class( list_t * array_pos ) {
	list_Init_Subrot( array_pos );
	list_Add_Subrot( array_pos, "new" );
	list_Add_Subrot( array_pos, "dispose" );
}

void list_Init_Output_Class( list_t * output_pos ) {
	list_Init_Subrot( output_pos );
	list_Add_Subrot( output_pos, "init" );
	list_Add_Subrot( output_pos, "moveCursor" );
	list_Add_Subrot( output_pos, "printChar" );
	list_Add_Subrot( output_pos, "printString" );
	list_Add_Subrot( output_pos, "printInt" );
	list_Add_Subrot( output_pos, "println" );
	list_Add_Subrot( output_pos, "backSpace" );
}

void list_Init_Screen_Class( list_t * screen_pos ) {
	list_Init_Subrot( screen_pos );
	list_Add_Subrot( screen_pos, "init" );
	list_Add_Subrot( screen_pos, "clearScreen" );
	list_Add_Subrot( screen_pos, "setColor" );
	list_Add_Subrot( screen_pos, "drawPixel" );
	list_Add_Subrot( screen_pos, "drawLine" );
	list_Add_Subrot( screen_pos, "drawRectangle" );
	list_Add_Subrot( screen_pos, "drawCircle" );
}

void list_Init_Keyboart_Class( list_t * keyboard_pos ) {
	list_Init_Subrot( keyboard_pos );
	list_Add_Subrot( keyboard_pos, "init" );
	list_Add_Subrot( keyboard_pos, "keyPressed" );
	list_Add_Subrot( keyboard_pos, "readChar" );
	list_Add_Subrot( keyboard_pos, "readLine" );
	list_Add_Subrot( keyboard_pos, "readInt" );
}

void list_Init_Memory_Class( list_t * memory_pos ) {
	list_Init_Subrot( memory_pos );
	list_Add_Subrot( memory_pos, "init" );
	list_Add_Subrot( memory_pos, "peek" );
	list_Add_Subrot( memory_pos, "poke" );
	list_Add_Subrot( memory_pos, "alloc" );
	list_Add_Subrot( memory_pos, "deAlloc" );
}

void list_Init_Sys_Class( list_t * sys_pos ) {
	list_Init_Subrot( sys_pos );
	list_Add_Subrot( sys_pos, "init" );
	list_Add_Subrot( sys_pos, "halt" );
	list_Add_Subrot( sys_pos, "error" );
	list_Add_Subrot( sys_pos, "wait" );
}
