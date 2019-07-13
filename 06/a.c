#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char str[] = "abcdefg\0";
	int i = 0;

	fprintf( stdout, "%c%c%c\n", str[i], str[(i++;)], str[i++] );

	//strtol( str, tp, 10 );
	//fprintf( stdout, "%s\n", str );
	//fprintf( stdout, "%d\n", strcmp( "hello", "he110" ) );

	return 9;
}
