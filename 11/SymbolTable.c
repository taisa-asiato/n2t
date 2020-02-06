#include "define.h"

void init_SymbolTable() {
	cls = malloc( sizeof( scope_t ) );
	strcpy( cls->name, "" );
	strcpy( cls->type, "" );
	strcpy( cls->proper, "" );
	cls->number = -1;
	cls->next = NULL;
	cls->prev = NULL;

	clsp = cls;
}

void init_SubroutineTable() {
	sub = malloc( sizeof( scope_t ) );
	strcpy( sub->name, "" );
	strcpy( sub->type, "" );
	strcpy( sub->proper, "" );
	sub->number = -1;
	sub->next = NULL;
	sub->prev = NULL;

	subp = sub;
}

void constructer() {
	init_SubroutineTable();
	init_SymbolTable();
	cnt_static = -1;
        cnt_field = -1;
        cnt_arg = -1;
        cnt_var = -1;
}

void my_define( int iscls, char * symbol_name, char * type, char * proper, int number ) {
	fprintf( stdout, "%lu\n", sizeof( scope_t ) );
	scope_t * tmp = malloc( sizeof( scope_t ) );
	strcpy( tmp->name, symbol_name );
	strcpy( tmp->type, type );
	strcpy( tmp->proper, proper);
	tmp->number = number;

	if ( iscls ) {
		clsp->next = tmp;
		tmp->prev = clsp;
		tmp->next = NULL;
		clsp = tmp;
	} else {
		subp->next = tmp;
		tmp->prev = subp;
		tmp->next = NULL;
		subp = tmp;
	}
	if ( debug ) {
		fprintf( stdout, "function %s finished\n", __func__  );
	}
}

scope_t * list_Find_Scope_Cls( char * symbol_name ) {
	scope_t * tmp = cls;
	while ( tmp != NULL ) {
		if ( strcmp( symbol_name, tmp->name ) == 0 ) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

scope_t * list_Find_Scope_Sub( char * symbol_name ) {
	scope_t * tmp = sub;
	while ( tmp != NULL ) {
		if ( strcmp( symbol_name, tmp->name ) == 0 ) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

scope_t * list_Find_Scope( char * symbol_name ) {
	scope_t * tmp;
	
	tmp = list_Find_Scope_Cls( symbol_name );
	if ( tmp ) {
		return tmp;
	} else {
		return list_Find_Scope_Sub( symbol_name );
	}
}

int var_Count( char * my_typeof ) {
	int cnt = 0;
	if ( strcmp( my_typeof, "static" ) == 0 || strcmp( my_typeof, "field" ) == 0 ) {
		scope_t * tmp = cls->next; 
		while ( tmp ) {
			if ( strcmp( tmp->type, my_typeof ) == 0 ) {
				cnt += 1;
			}
			tmp = tmp->next;
		}
	} else if ( strcmp( my_typeof, "var" ) == 0 || strcmp( my_typeof, "argument" ) == 0 ) {
		scope_t * tmp = sub->next;
		while ( tmp ) {
			if ( strcmp( tmp->type, my_typeof ) == 0 ) {
				cnt += 1;
			}
			tmp = tmp->next;
		}
	}
	return cnt;
}

int kind_Of( char * name ) {
	
	if ( debug ) {
		fprintf(stdout, "[%s]\n", __func__);
	}
	
	if ( strcmp( name, "static" ) == 0 ) {
		return STATIC;
	} else if ( strcmp( name, "field" ) == 0 ) {
		return FIELD;
	} else if ( strcmp( name, "argument" ) == 0 ) {
		return ARG;
	} else if ( strcmp( name, "var" ) == 0 ) {
		return VAR;
	}
	return NONE;
}

int type_Of( char * name ) {
	scope_t * tmp;
	if ( debug ) {
		fprintf(stdout, "[%s]\n", __func__);
	}

	tmp = list_Find_Scope( name );
	if ( tmp ) {
		if ( strcmp( tmp->type, "int" ) == 0 ) {
			return INT;
		} else if ( strcmp( tmp->type, "boolean" ) == 0 ) {
			return BOOLEAN;
		} else if ( strcmp( tmp->type, "char" ) == 0 ) {
			return CHAR;
		} else if ( strcmp( tmp->type, "void" ) == 0 ) {
			return VOID;
		} else if ( list_Find_Node( tmp->type ) == 0 ) {
			return CLASS;
		}
	} 
	return -1;
}

int index_Of( char * name ) {
	if ( debug ) {
		fprintf(stdout, "[%s]\n", __func__);
	}
	scope_t * tmp = list_Find_Scope( name );
	
	if (debug) {
		fprintf(stdout, "[%s] addresss is [%p], index is [%d]\n", name, tmp, tmp->number);
	}
	return tmp->number;
}

void del_SymbolTable() {
	scope_t * tmp = cls;
	scope_t * del;

	while ( tmp ) {
		del = tmp;
		tmp = tmp->next;
		free( del );
	}
}

void del_SubroutineTable() {
	scope_t * tmp = sub;
	scope_t * del;

	while ( tmp ) {
		del = tmp;
		tmp = tmp->next;
		free( del );
	}
}

void print_All_Symbol( char * funcname ) {
	scope_t * tmp = cls;
	fprintf(stdout, "==> [Class_Scope] <==\n");
	fprintf( stdout, "[%s]\n", funcname );
	while ( tmp ) {
		fprintf(stdout, "%10s(name)\n", tmp->name );
		fprintf(stdout, "%10s(type)\n", tmp->type );
		fprintf(stdout, "%10s(property)\n", tmp->proper );
		fprintf(stdout, "%10d(index)\n", tmp->number);
		fprintf(stdout, "==\n");
		tmp = tmp->next;
	}

	fprintf(stdout, "==> [Subrot_Scope] <==\n");
	tmp = sub;
	while ( tmp ) {
		fprintf(stdout, "%10s(name)\n", tmp->name );
		fprintf(stdout, "%10s(type)\n", tmp->type );
		fprintf(stdout, "%10s(property)\n", tmp->proper );
		fprintf(stdout, "%10d(index)\n", tmp->number);
		fprintf(stdout, "==\n");
		tmp = tmp->next;
	}
}
