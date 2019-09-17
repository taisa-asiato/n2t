#include "define.h"

int gen_inputfilename( struct dirent * dp, char * dirname ) {
	int isjack = 0;
	char tmpfilename[256];
	char * cp;

	if ( strstr( dp->d_name, ".jack" ) ) {
		strcpy( tmpfilename, dp->d_name );
		sprintf( streamfilename, "%s%s", dirname, dp->d_name );
		isjack = 1;
	}
	return isjack;
}

void gen_outputfilename( char * filename ) {
	char tmpfilename[256];
	char * cp;

	strcpy( tmpfilename, filename );
	cp = strstr( tmpfilename, ".jack" );
	*cp = '\0';
	sprintf( outputfilename, "%s.xml", tmpfilename );
}

void jack_analyze_main( char * fname ) {
	DIR * dirp;
	struct dirent * dp;

	if ( ( dirp = opendir( fname ) ) != NULL ) {
		// 入力ファイルがディレクトリの場合
		while ( ( dp = readdir( dirp ) ) != NULL ) {
			// 入力ファイル名を作成
			if ( gen_inputfilename( dp, fname ) ) {
				fprintf( stdout, "%s\n", streamfilename );
				if ( ( inputfp = fopen( streamfilename, "r"  ) ) ) {
					gen_outputfilename( streamfilename );
					fprintf( stdout, "=> output is %s\n", outputfilename );
					// outputfp = fopen( outputfilename, "w" );
					// jack_tokenizer_main( outputfilename );
					// fclose( outputfp );
					fclose( inputfp );
				}
			}
		}
		fprintf( stdout, "under construction\n" );
	} else {
		// 入力ファイルがディレクトリでない場合
		strcpy( streamfilename, fname );
		fprintf( stdout, "<= %s\n", streamfilename );
		if ( ( inputfp = fopen( streamfilename, "r"  ) ) ) {
			gen_outputfilename( streamfilename );
			fprintf( stdout, "=> output is %s\n", outputfilename );
			// outputfp = fopen( outputfilename, "w" );
			// jack_tokenizer_main_main( outputfilename ); 
			// fclose( outputfp );
			fclose( inputfp );
		}
	}
}
