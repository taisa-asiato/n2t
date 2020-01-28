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

void my_define( int iscls, char * symbol_name, int type, int proper, int number ) {
	scope_t * tmp = malloc( sizoef( scope_t ) );
	strcpy( tmp->name, symbol_name );
	tmp->type = type;
	tmp->proper = proper;
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
