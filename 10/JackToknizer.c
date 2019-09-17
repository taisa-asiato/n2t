#include "define.h" 

void jack_tokenizer_main( FILE * ifp, FILE * ofp  ) {
	char current_line[256];

	/*
	if ( has_more_commands( ifp ) == true ) {
		// 
		advance();
	} else {
	
	} 
	*/
}

bool has_more_commands( FILE * fp ) {
	if ( fgets( streamline, sizeof( streamline ) / sizeof( char ), fp ) != EOF ) {
		true;
	} else {
		false;
	}
}

void advance() {
	char * cp;

	if ( cp = strtok( streamline, "//" ) ) {
		// コメントアウト以降の文字削除
		*cp = '\0';
	}

	// if ( streamline[0] )

}
