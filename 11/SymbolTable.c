#include "define.h"

void init_SymbolTable() {
	cls = malloc( sizoef( scope_t ) );
	strcpy( cls->name, "" );
	cls->type = -1;
	cls->proper = -1;
	cls->number = -1;
	cls->next = NULL;
	cls->prev = NULL;

	clsp = cls;
}

void init_SubroutineTable {
	sub = malloc( sizoef( scope_t ) );
	strcpy( sub->name, "" );
	sub->type = -1;
	sub->proper = -1;
	sub->number = -1;
	sub->next = NULL;
	sub->prev = NULL;

	subp = sub;
}

void construter() {
	init_SubroutineTable();
	init_SymbolTable();
}

void my_define( int iscls, char * symbol_name, char * type, char * proper, int number ) {
	scope_t * tmp = malloc( sizoef( scope_t ) );
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
	scope_t * tmp = subp;
	while ( tmp != NULL ) {
		if ( strcmp( symbol_name, tmp->name ) ) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL
}

scope_t * list_Find_Scope( char * symbol_name ) {
	scope_t * tmp;

	if ( tmp = list_Find_Scope_Cls( symbol_name ) ) {
		return tmp;
	} else {
		return list_Find_Scope_Sub( symbol_name );
	}
}

int var_Count( char * typeof ) {
	int cnt = 0;
	if ( strcmp( typeof, "static" ) == 0 || strcmp( typeof, "field" ) == 0 ) {
		scope_t * tmp = cls->next; 
		while ( tmp ) {
			if ( strcmp( tmp->type, typeof ) == 0 ) {
				cnt += 1;
			}
			tmp = tmp->next;
		}
	} else if ( strcmp( typeof, "var" ) == 0 || strcmp( typeof, "argument" ) == 0 ) {
		scope_t * tmp = sub->next;
		while ( tmp ) {
			if ( strcmp( tmp->type, typeof ) == 0 ) {
				cnt += 1;
			}　
			tmp = tmp->next;
		}
	}
	return cnt;
}
