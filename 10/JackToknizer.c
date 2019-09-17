#include "define.h" 

void jack_tokenizer_main( FILE * ifp, FILE * ofp  ) {
	char current_line[256];
	char * cp;

	while ( fgets( streamline, ( sizeof( streamline )/sizeof( char ) ), ifp ) != EOF ){
		cp = strtok( streamline, " " );
		while ( has_more_tokens( cp ) ) {
			advance( cp );
			cp = strtok( NULL, " " );
		}
	} 
}

bool has_more_tokens( char * istoken ) {
	if ( strcmp( istoken, NULL ) != 0 ) {
		true;
	} else {
		false;
	}
}

void advance( char * cp ) {
	// 次のトークンに進める
	strcpy( token, cp );
}

int token_type( char current[256] ) {
		
	
	if ( is_keyword( current ) ) {
		return KEYWORD;
	} else if ( is_symbol( current ) ) {
		return SYMBOL;
	} else if ( is_integer_constant( current ) ) {
		return INT_CONST;
	} else if ( is_string_constant( current ) ) {
		return STRING_CONST;
	} else if ( is_identifier( current ) )  {
		return IDENTFIER;
	} else {
		return -1;
	}
}

bool is_keyword( char c_token[256] ) {
	if ( strcpy( ) )
}
